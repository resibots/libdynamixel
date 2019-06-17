#ifndef DYNAMIXEL_CONTROLLERS_USB2DYNAMIXEL_HPP_
#define DYNAMIXEL_CONTROLLERS_USB2DYNAMIXEL_HPP_

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "../errors/error.hpp"
#include "../instruction_packet.hpp"
#include "../misc.hpp"
#include "../status_packet.hpp"

namespace dynamixel {
    /** Give an explanation for the error number associated to `write`.

        @param error number, usually the value of errno
        @return std::string of the explanation
    **/
    inline std::string write_error_string(int error_number)
    {
        switch (error_number) {
        case EAGAIN:
            return "EAGAIN: The file descriptor fd refers to a file other than "
                   "a socket and has been marked nonblocking (O_NONBLOCK), and "
                   "the write would block.";
        // On some OS, EWOULDBLOCK has the same value as EAGAIN and would hence not compile.
        // case EWOULDBLOCK:
        //     return "EWOULDBLOCK: The file descriptor fd refers to a socket "
        //            "and has been marked nonblocking (O_NONBLOCK), and the "
        //            "write would block.";
        case EBADF:
            return "EBADF: fd is not a valid file descriptor or is not open for writing.";
        case EDESTADDRREQ:
            return "EDESTADDRREQ: fd refers to a datagram socket for which a "
                   "peer address has not been set using connect(2).";
        case EDQUOT:
            return "EDQUOT: The user's quota of disk blocks on the file system "
                   "containing the file referred to by fd has been exhausted.";
        case EFAULT:
            return "EFAULT: buf is outside your accessible address space.";
        case EFBIG:
            return "EFBIG: An attempt was made to write a file that exceeds the "
                   "implementation-defined maximum file size or the process's "
                   "file size limit, or to write at a position past the maximum "
                   "allowed offset.";
        case EINTR:
            return "EINTR: The call was interrupted by a signal before any data "
                   "was written; see signal(7).";
        case EINVAL:
            return "EINVAL: fd is attached to an object which is unsuitable for "
                   "writing; or the file was opened with the O_DIRECT flag, and "
                   "either the address specified in buf, the value specified in "
                   "count, or the current file offset is not suitably aligned.";
        case EIO:
            return "EIO: A low-level I/O error occurred while modifying the inode.";
        case ENOSPC:
            return "ENOSPC: The device containing the file referred to by fd "
                   "has no room for the data.";
        case EPIPE:
            return "EPIPE: fd is connected to a pipe or socket whose reading end is closed.";
        }
        return "";
    }

    namespace controllers {
        class Usb2Dynamixel {
            // TODO : declare private copy constructor and assignment operator
        public:
            Usb2Dynamixel(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1)
                : _recv_timeout(recv_timeout), _fd(-1), _report_bad_packet(false)
            {
                open_serial(name, baudrate);
            }

            Usb2Dynamixel()
                : _recv_timeout(0.1), _fd(-1), _report_bad_packet(false) {}

            ~Usb2Dynamixel()
            {
                close_serial();
            }

            void open_serial(const std::string& name, int baudrate = B115200)
            {
                struct termios tio_serial;

                // Firstly, check that there is no active connexion
                if (_fd != -1)
                    throw errors::Error("error attempting to open device " + name + ": an other connection is active; call `close serial` before opening a new connection");

                _fd = open(name.c_str(), O_RDWR | O_NOCTTY);
                if (_fd == -1)
                    throw errors::Error("error opening device " + name + ": " + std::string(strerror(errno)));

                // Serial port setting
                bzero(&tio_serial, sizeof(tio_serial));
                tio_serial.c_cflag = CS8 | CLOCAL | CREAD;
                tio_serial.c_iflag = IGNBRK | IGNPAR;
                tio_serial.c_oflag = 0;
                tio_serial.c_lflag = 0;

                // could be 1
                tio_serial.c_cc[VMIN] = 0;

                cfsetispeed(&tio_serial, baudrate);
                cfsetospeed(&tio_serial, baudrate);

                cfgetispeed(&tio_serial);
                tcflush(_fd, TCIFLUSH);
                tcsetattr(_fd, TCSANOW, &tio_serial);
            }

            void close_serial()
            {
                // apparently the mac does not flush everything
                // before closing the fd (Linux does)
                //... which can cause the last packet to be ignored
#ifdef __APPLE__
                usleep(1000);
#endif
                close(_fd);
                _fd = -1;
            }

            bool is_open() { return !(_fd == -1); }

            void flush() { tcflush(_fd, TCIFLUSH); }

            double recv_timeout() { return _recv_timeout; }

            void set_recv_timeout(double recv_timeout) { _recv_timeout = recv_timeout; }

            // general send
            template <typename T>
            void send(const InstructionPacket<T>& packet) const
            {
                if (_fd == -1)
                    return;

                const int ret = write(_fd, packet.data(), packet.size());

                //  std::cout << "Send: ";
                //  for (size_t i = 0; i < packet.size(); ++i)
                //      std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)packet[i] << " ";
                //  std::cout << std::endl;

                if (ret == -1) {
                    throw errors::Error("Usb2Dynamixel::Send write error " + write_error_string(errno));
                }
                else if (ret != packet.size()) {
                    std::stringstream ofs;
                    perror("write:");
                    ofs << "written= " << ret << " size=" << packet.size();
                    throw errors::Error("Usb2Dynamixel::Send: packet not fully written " + ofs.str());
                }
            }

            // general receive
            template <typename T>
            bool recv(StatusPacket<T>& status) const
            {
                using DecodeState = typename T::DecodeState;

                if (_fd == -1)
                    return false;

                double time = get_time();
                DecodeState state = DecodeState::ONGOING;

                std::vector<uint8_t> packet;
                packet.reserve(_recv_buffer_size);

                //  std::cout << "Receive:" << std::endl;

                do {
                    double current_time = get_time();
                    uint8_t byte;
                    int res = read(_fd, &byte, 1);
                    if (res > 0) {
                        //  std::cout << std::setfill('0') << std::setw(2)
                        //            << std::hex << (unsigned int)byte << " ";
                        packet.push_back(byte);

                        state = status.decode_packet(packet, _report_bad_packet);
                        if (state == DecodeState::INVALID) {
                            //  std::cout << "\tBad packet: ";
                            //  for (const auto byte : packet)
                            //      std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)byte << " ";
                            //    std::cout << std::endl;

                            packet.clear();
                        }

                        time = current_time;
                    }

                    if (current_time - time > _recv_timeout)
                        return false;
                } while (state != DecodeState::DONE);

                //  std::cout << std::endl;
                //  std::cout << std::dec;

                return true;
            }

            /** Enable error reporting for packet issues

                If report_bad_packet is set to true, invalid packet headers and
                errors in packet size are reported through exceptions.
            **/
            void set_report_bad_packet(bool report_bad_packet)
            {
                _report_bad_packet = report_bad_packet;
            }

            /**
                @see set_report_bad_packet
            **/
            bool report_bad_packet()
            {
                return _report_bad_packet;
            }

        private:
            double _recv_timeout;
            static const size_t _recv_buffer_size = 256;
            int _fd;
            bool _report_bad_packet;
        };
    } // namespace controllers
} // namespace dynamixel

#endif
