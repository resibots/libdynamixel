#ifndef DYNAMIXEL_CONTROLLERS_FILE2DYNAMIXEL_HPP_
#define DYNAMIXEL_CONTROLLERS_FILE2DYNAMIXEL_HPP_

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
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
        /** Class to emulate communication with servos.

            By using a file, we can emulate sending packets and receiving them.
            This class is used for testing and debugging only.
        **/
        class File2Dynamixel {
            // TODO : declare private copy constructor and assignment operator
        public:
            File2Dynamixel(const std::string& name)
                : _fd(-1), _report_bad_packet(false)
            {
                open_file(name);
            }

            File2Dynamixel(const std::string& name, int flags)
                : _fd(-1), _report_bad_packet(false)
            {
                open_file(name, flags);
            }

            File2Dynamixel(const std::string& name, int flags, mode_t mode)
                : _fd(-1), _report_bad_packet(false)
            {
                open_file(name, flags, mode);
            }

            File2Dynamixel() : _fd(-1), _report_bad_packet(false) {}

            ~File2Dynamixel()
            {
                close_file();
            }

            void open_file(const std::string& name,
                int flags = (O_RDWR | O_CREAT | O_APPEND),
                mode_t mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH))
            {
                if (_fd != -1)
                    throw errors::Error("error attempting to open file " + name + ": an other file is open; call `close_file` before opening a new file");

                // open the file
                _fd = open(name.c_str(), flags, mode);
                if (_fd == -1)
                    throw errors::Error("error opening file " + name + ": " + std::string(strerror(errno)));
            }

            void close_file()
            {
                close(_fd);
                _fd = -1;
            }

            bool is_open() { return !(_fd == -1); }

            off_t seek(unsigned int offset)
            {
                return lseek(_fd, (off_t)offset, SEEK_SET);
            }

            // send for instruction packets
            template <typename T>
            void send(const InstructionPacket<T>& packet) const
            {
                if (_fd == -1)
                    return;

                int ret = write(_fd, packet.data(), packet.size());

                std::cout << "Send: ";
                for (size_t i = 0; i < packet.size(); ++i)
                    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)packet[i] << " ";
                std::cout << std::endl;

                if (ret != packet.size()) {
                    std::stringstream ofs;
                    perror("write:");
                    ofs << "written= " << ret << ", size=" << packet.size();
                    throw errors::Error("File2Dynamixel::Send: packet not fully written" + ofs.str());
                }
            }

            // generic send
            void send(const std::vector<uint8_t>& packet) const
            {
                if (_fd == -1)
                    return;

                int ret = write(_fd, &packet.front(), packet.size());

                std::cout << "Send: ";
                for (size_t i = 0; i < packet.size(); ++i)
                    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)packet[i] << " ";
                std::cout << std::endl;

                if (ret != packet.size()) {
                    std::stringstream ofs;
                    perror("write:");
                    ofs << "written= " << ret << ", size=" << packet.size();
                    throw errors::Error("File2Dynamixel::Send: packet not fully written" + ofs.str());
                }
            }

            // general receive
            template <typename T>
            bool recv(StatusPacket<T>& status) const
            {
                using DecodeState = typename T::DecodeState;

                if (_fd == -1)
                    return false;

                int res = 0;
                DecodeState state = DecodeState::ONGOING;

                std::vector<uint8_t> packet;
                packet.reserve(_recv_buffer_size);

                std::cout << "Receive:" << std::endl;

                do {
                    uint8_t byte;
                    res = read(_fd, &byte, 1);

                    if (res > 0) {
                        // std::cout << std::setfill('0') << std::setw(2)
                        //           << std::hex << (unsigned int)byte << " ";
                        packet.push_back(byte);

                        state = status.decode_packet(packet, _report_bad_packet);
                        if (state == DecodeState::INVALID) {
                            std::cout << "\tBad packet: ";
                            for (const auto byte : packet)
                                std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)byte << " ";
                            std::cout << std::endl;

                            packet.clear();
                        }
                        else if (state == DecodeState::DONE) {
                            std::cout << "\tGood packet: ";
                            for (const auto byte : packet)
                                std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)byte << " ";
                            std::cout << std::endl;
                        }
                    }
                } while (state != DecodeState::DONE && res);

                // std::cout << "Status in the end is ";
                // switch (state) {
                // case DecodeState::INVALID:
                //     std::cout << "INVALID";
                //     break;
                // case DecodeState::ONGOING:
                //     std::cout << "ONGOING";
                //     break;
                // case DecodeState::DONE:
                //     std::cout << "DONE";
                //     break;
                // }
                std::cout << std::endl;

                //std::cout << std::endl;
                std::cout << std::dec;

                return true;
            }

            void set_report_bad_packet(bool report_bad_packet)
            {
                _report_bad_packet = report_bad_packet;
            }

            bool report_bad_packet()
            {
                return _report_bad_packet;
            }

        private:
            static const size_t _recv_buffer_size = 256;
            int _fd;
            bool _report_bad_packet;
        };
    }
}

#endif
