#ifndef DYNAMIXEL_USB2DYNAMIXEL_HPP_
#define DYNAMIXEL_USB2DYNAMIXEL_HPP_

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

#include "error.hpp"
#include "misc.hpp"
#include "packet.hpp"
//#include "status.hpp"

namespace dynamixel {
    static const byte_t broadcast = 0xfe;

    class Usb2Dynamixel {
    public:
        Usb2Dynamixel(const std::string& name, int baudrate = B115200)
        {
            open_serial(name, baudrate);
        }

        Usb2Dynamixel() : _fd(-1) {}

        void open_serial(const std::string& name, int baudrate = B115200)
        {
            struct termios tio_serial;

            _fd = open(name.c_str(), O_RDWR | O_NOCTTY);
            if (_fd == -1)
                throw Error("error opening device:" + name + " " + std::string(strerror(errno)));

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
            // cfsetispeed(&tio_serial, B115200);
            // cfsetospeed(&tio_serial, B115200);

            int res = cfgetispeed(&tio_serial);
            tcflush(_fd, TCIFLUSH);
            tcsetattr(_fd, TCSANOW, &tio_serial);
        }

        void close_serial()
        {
            close(_fd);
            _fd = -1;
        }

        bool is_open() { return !(_fd == -1); }

        void flush() { tcflush(_fd, TCIFLUSH); }

        // general send
        template <typename T>
        void send(const Packet<T>& packet)
        {
            if (_fd == -1)
                return;

            int ret = write(_fd, &packet.packet().front(), packet.packet().size());
            /*    std::cout << "Send: ";
          for (size_t i = 0; i < inst.packet_size(); ++i)
            std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex <<
         (unsigned int)inst.packet()[i] << " ";
          std::cout << std::endl;*/

            if (ret != packet.packet().size()) {
                std::stringstream ofs;
                perror("write:");
                ofs << "written= " << ret << " size=" << packet.packet().size();
                throw Error("Usb2Dynamixel::Send: packet not fully written" + ofs.str());
            }
        }

        // general receive
        //bool recv(float timeout, Status& status);
    private:
        int _fd;
        static const size_t _recv_buffer_size = 256;
        uint8_t _recv_buffer[_recv_buffer_size];
    };
}

#endif
