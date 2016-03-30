#ifndef DYNAMIXEL_UTILITY
#define DYNAMIXEL_UTILITY

#include <dynamixel/dynamixel.hpp>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;
using namespace protocols;

namespace dynamixel {
    class Utility {
    public:
        Utility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1) : _serial_interface(name, baudrate, recv_timeout)
        {
            _servos = auto_detect<Protocol1>(_serial_interface);
        }

        std::vector<std::shared_ptr<BaseServo<Protocol1>>>& list()
        {
            return _servos;
        }

    private:
        Usb2Dynamixel _serial_interface;
        std::vector<std::shared_ptr<BaseServo<Protocol1>>> _servos;
    };
}

#endif