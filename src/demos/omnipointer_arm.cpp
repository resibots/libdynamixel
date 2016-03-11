#include <iostream>
#include <cstdlib>

#include <dynamixel/dynamixel_core.hpp>
#include <dynamixel/servos/mx106.hpp>
#include <dynamixel/servos/mx28.hpp>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;
using namespace protocols;

int main(int argc, char** argv)
{
    if (argc != 6) {
        std::cout << "Usage: " << argv[0] << " device angle_1 angle_2 angle_3 angle_4" << std::endl;
        return -1;
    }

    try {
        Usb2Dynamixel controller(argv[1], B1000000, 0.01);
        std::vector<double> rads(4);
        rads[0] = std::atof(argv[2]);
        rads[1] = std::atof(argv[3]);
        rads[2] = std::atof(argv[4]);
        rads[3] = std::atof(argv[5]);

        StatusPacket<Protocol1> status;

        for (int i = 0; i < 2; i++) {
            controller.send(Mx106::set_moving_speed(i + 1, 50));
            controller.recv(status);
            controller.send(Mx106::reg_goal_position_angle(i + 1, rads[i]));
            controller.recv(status);
        }

        for (int i = 2; i < 4; i++) {
            controller.send(Mx28::set_moving_speed(i + 1, 50));
            controller.recv(status);
            controller.send(Mx28::reg_goal_position_angle(i + 1, rads[i]));
            controller.recv(status);
        }

        controller.send(Action<Protocol1>(Protocol1::broadcast_id));
    }
    catch (const errors::Error& e) {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
