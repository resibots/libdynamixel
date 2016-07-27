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
        // Default baudrate for the dynamixel pro is 57600
#ifndef __APPLE__
        Usb2Dynamixel controller(argv[1], B1000000, 0.01);
#else
        Usb2Dynamixel controller(argv[1], B115200, 0.01);
#endif
        std::vector<std::shared_ptr<BaseServo<Protocol1>>> servos_1 = auto_detect<Protocol1>(controller);
        std::vector<std::shared_ptr<BaseServo<Protocol2>>> servos_2 = auto_detect<Protocol2>(controller);

        if (servos_1.size()) {
            std::cout << "Detected an " << servos_1[0]->model_name() << " with ID " << servos_1[0]->id() << std::endl;

            StatusPacket<Protocol1> st;

            controller.send(servos_1[0]->set_torque_enable(1));
            controller.recv(st);

            for (double angle = M_PI / 2; angle < (3 * M_PI / 2) + 1e-6; angle += M_PI / 6) {
                std::cout << "Setting position " << angle << " radians" << std::endl;
                controller.send(servos_1[0]->set_goal_position_angle(angle));
                controller.recv(st);
                std::cin.get();
            }
        }
        else if (servos_2.size()) {
            std::cout << "Detected an " << servos_2[0]->model_name() << " with ID " << servos_2[0]->id() << std::endl;

            StatusPacket<Protocol2> st;

            controller.send(servos_2[0]->set_torque_enable(1));
            controller.recv(st);

            for (double angle = M_PI / 2; angle < (3 * M_PI / 2) + 1e-6; angle += M_PI / 6) {
                std::cout << "Setting position " << angle << " radians" << std::endl;
                controller.send(servos_2[0]->set_goal_position_angle(angle));
                controller.recv(st);
                std::cin.get();
            }
        }
        else {
            std::cout << "No dynamixel detected" << std::endl;
        }
    }
    catch (const errors::Error& e) {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
