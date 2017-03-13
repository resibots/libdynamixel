#include <iostream>
#include <cmath>

#include <dynamixel/dynamixel.hpp>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;
using namespace protocols;

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " device" << std::endl;
        return -1;
    }

    try {
#ifndef __APPLE__
        Usb2Dynamixel controller(argv[1], B1000000, 0.01);
#else
        Usb2Dynamixel controller(argv[1], B115200, 0.01);
#endif
        std::vector<std::shared_ptr<BaseServo<Protocol1>>> servos = auto_detect<Protocol1>(controller);
        StatusPacket<Protocol1> st;

        for (auto servo : servos) {
            std::cout << "Detected an " << servo->model_name() << " with ID " << servo->id() << std::endl;
            controller.send(servo->set_torque_enable(1));
            controller.recv(st);
            controller.send(servo->set_moving_speed_angle(1));
            controller.recv(st);
            controller.send(servo->set_goal_position_angle(M_PI));
            controller.recv(st);
        }
    }
    catch (const errors::Error& e) {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
