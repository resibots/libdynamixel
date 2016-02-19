#include <iostream>
#include <cstdlib>

#include <dynamixel/dynamixel.hpp>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;
using namespace protocols;

void set_positions(const Usb2Dynamixel& controller, const std::vector<double>& deg)
{
    StatusPacket<Protocol1> status;
    for (int i = 0; i < 2; i++){
        controller.send(Mx106::set_moving_speed(i+1, 50));
        controller.recv(status);
        controller.send(Mx106::reg_goal_position_angle(i+1, deg[i]));
        controller.recv(status);
    }

    for (int i = 2; i < 4; i++){
        controller.send(Mx28::set_moving_speed(i+1, 50));
        controller.recv(status);
        controller.send(Mx28::reg_goal_position_angle(i+1, deg[i]));
        controller.recv(status);
    }

    controller.send(Action<Protocol1>(Protocol1::broadcast_id));
}

int main(int argc, char** argv)
{
    if (argc != 6)
    {
        std::cout << "Usage: " << argv[0] << " device angle_1 angle_2 angle_3 angle_4" << std::endl;
        return -1;
    }

    Usb2Dynamixel controller(argv[1], B1000000, 0.1);
    std::vector<double> deg(4);
    deg[0] = std::atof(argv[2]) * 57.295779513;
    deg[1] = std::atof(argv[3]) * 57.295779513;
    deg[2] = std::atof(argv[4]) * 57.295779513;
    deg[3] = std::atof(argv[5]) * 57.295779513;

    try
    {
        set_positions(controller, deg);
    }
    catch (const errors::Error& e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
