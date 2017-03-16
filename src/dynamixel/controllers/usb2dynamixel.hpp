#ifndef DYNAMIXEL_CONTROLLERS_USB2DYNAMIXEL_HPP_
#define DYNAMIXEL_CONTROLLERS_USB2DYNAMIXEL_HPP_

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "../errors/error.hpp"
#include "../misc.hpp"
#include "../instruction_packet.hpp"
#include "../status_packet.hpp"

namespace dynamixel {
    namespace controllers {
        class Usb2Dynamixel {
            // TODO : declare private copy constructor and assignment operator
        public:
            /** With this constructor a serial connection is opened or it
                throws an exception

                @see open_serial

                @param name see open_serial
                @param baudrate see open_serial
                @param recv_timeout timeout (in seconds) for the recv method

                @throws errors::Error see open_serial
            **/
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

            /** Open the serial port for communication with the servos

                @param name path to the UNIX serial port, usually ends like "ttyUSBx"
                    or "ttyACMx" where x is an integer
                @param baudrate represent the communication speed; it is defined
                    by the operating system. the standard values are:
                    \verbatim embed:rst

                        * B0
                        * B50
                        * B75
                        * B110
                        * B134
                        * B150
                        * B200
                        * B300
                        * B600
                        * B1200
                        * B1800
                        * B2400
                        * B4800
                        * B9600
                        * B19200
                        * B38400
                        * B57600
                        * B76800
                        * B115200

                    B1000000 corresponds to :math:`10^6` bauds. It is not a standard
                    POSIX value but is accepted on GNU/Linux.
                    \endverbatim

                @throws errors::Error either if the connection is already open
                    or if we fail to open it; the exception message gives more
                    details on the cause
            **/
            void open_serial(const std::string& name, int baudrate = B115200)
            {
                struct termios tio_serial;

                // Firstly, check that there is no active connexion
                if (_fd != -1)
                    throw errors::Error("error attempting to open device " + name + ": an other connexion is active; call `close serial` before opening a new connexion");

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

            /** Close the serial port
            **/
            void close_serial()
            {
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

                int ret = write(_fd, packet.data(), packet.size());

                /*std::cout << "Send: ";
            for (size_t i = 0; i < packet.size(); ++i)
                std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)packet[i] << " ";
            std::cout << std::endl;*/

                if (ret != packet.size()) {
                    std::stringstream ofs;
                    perror("write:");
                    ofs << "written= " << ret << " size=" << packet.size();
                    throw errors::Error("Usb2Dynamixel::Send: packet not fully written" + ofs.str());
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

                //std::cout << "Receive:" << std::endl;

                do {
                    double current_time = get_time();
                    uint8_t byte;
                    int res = read(_fd, &byte, 1);
                    if (res > 0) {
                        // std::cout << std::setfill('0') << std::setw(2)
                        //           << std::hex << (unsigned int)byte << " ";
                        packet.push_back(byte);

                        state = status.decode_packet(packet, _report_bad_packet);
                        if (state == DecodeState::INVALID) {
                            // std::cout << "\tBad packet: ";
                            // for (const auto byte : packet)
                            //     std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)byte << " ";
                            // std::cout << std::endl;

                            packet.clear();
                        }

                        time = current_time;
                    }

                    if (current_time - time > _recv_timeout)
                        return false;
                } while (state != DecodeState::DONE);

                //std::cout << std::endl;
                std::cout << std::dec;

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
    }
}

#endif
