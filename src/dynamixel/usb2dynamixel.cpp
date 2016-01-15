#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include <cstdio>
#include "usb2dynamixel.hpp"
#include "ax12.hpp"

namespace dynamixel {
    void Usb2Dynamixel::open_serial(const std::string& name, int baudrate)
    

    void Usb2Dynamixel::send(const Instruction& inst)
    

    bool Usb2Dynamixel::recv(float timeout, Status& st)
    {
        if (_fd == -1)
            return true;
        int p = 0;
        double time = get_time();
        bool done = false;
        memset(_recv_buffer, 0, _recv_buffer_size);
        // std::cout << "Recv: ";
        do {
            double current_time = get_time();
            byte_t b;
            int res = read(_fd, &b, 1);
            if (res > 0) {
                _recv_buffer[p++] = b;
                time = current_time;
                //    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex
                //    << (unsigned int)b << " ";
                done = st.decode_packet(_recv_buffer, p);
            }
            if (current_time - time > timeout) {
                // std::cout << std::endl;
                return false;
            }
        } while (!done);

        // std::cout << std::endl;

        return true;
    }

    void Usb2Dynamixel::scan_ax12s()
    {
        if (_fd == -1)
            return;
        Status st;
        for (size_t i = 0; i < 253; ++i) {
            send(ax12::Ping(i));
            if (recv(0.01, st)) // TODO tune this number
                _ax12_ids.push_back(st.get_id());
        }
    }
}
