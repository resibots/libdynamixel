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
