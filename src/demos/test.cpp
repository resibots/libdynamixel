#include <iostream>
#include <iomanip>
#include <dynamixel/dynamixel.hpp>
#include <cmath>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;

#define RECV_TIME 0.01


std::vector<unsigned int> ids(1);
std::vector<unsigned int> positions(1);
std::vector<unsigned int> speeds(1);


template <typename Protocol>
void find_devices(int baudrate)
{
    Usb2Dynamixel controller("/dev/ttyUSB0", baudrate);
    StatusPacket<Protocol> status;

    for (int i = 0; i < 254; i++) {
        controller.send(Ping<Protocol>(i));
        if (controller.recv(RECV_TIME, status) && status.valid() && status.id() == i) {
            controller.send(Read<Protocol>(i, 0, 2));
            controller.recv(0.1, status);
            uint16_t model;
            Protocol::unpack_data(status.parameters(), model);
            std::cout << "Found device. Baudrate: " << baudrate << " Id: " << i << " Model: " << model << std::endl;
        }
    }
}

template <typename Model>
void get_model_number(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::get_model_number(id));
    controller.recv(0.1, status);
    std::cout << "Id: " << id << " Model: " << Model::parse_model_number(status) << " Expected: " << Model::ct_t::model_number_value << std::endl;
}

template <typename Model>
void get_torque_enabled(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::get_torque_enable(id));
    controller.recv(0.1, status);
    std::cout << "Id: " << id << " Torque Enabled: " << (bool)Model::parse_torque_enable(status) << std::endl;
}

template <typename Model>
void get_max_torque(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::get_max_torque(id));
    controller.recv(0.1, status);
    std::cout << "Id: " << id << " Max Torque: " << Model::parse_max_torque(status) << std::endl;
}

template <typename Model>
void set_torque_enabled(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_torque_enable(id, true));
    controller.recv(0.1, status);
}

template <typename Model>
void set_max_torque(const Usb2Dynamixel& controller, int id, int value)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_max_torque(id, value));
    controller.recv(0.1, status);
}

template <typename Model>
void set_pos_to_zero(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_goal_position(id, 0));
    controller.recv(0.1, status);
}

template <typename Model>
void set_pos_to_max(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_goal_position(id, Model::ct_t::max_goal_position));
    controller.recv(0.1, status);
}

template <typename Model>
void set_vel_to_zero(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_moving_speed(id, 0));
    controller.recv(0.1, status);
}

template <typename Model>
void set_to_angle(const Usb2Dynamixel& controller, int id, double deg)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::set_goal_position_angle(id, deg));
    controller.recv(0.1, status);
}

template <typename Model>
void get_angle_pos(const Usb2Dynamixel& controller, int id)
{
    StatusPacket<typename Model::protocol_t > status;
    controller.send(Model::get_present_position_angle(id));
    controller.recv(0.1, status);
    std::cout << "Id: " << id << " Angle: " << Model::parse_present_position_angle(status) << "°" << std::endl;
}

template <typename Model>
void  test(const Usb2Dynamixel& controller, int id)
{
        get_model_number<Model>(controller, id);
        set_torque_enabled<Model>(controller, id);
        set_vel_to_zero<Model>(controller, id);

        std::cin.get();

        for (int i = 0; i <= 360; i += 15){
            if (i < Model::ct_t::min_goal_angle_deg)
                continue;
            if (i > Model::ct_t::max_goal_angle_deg)
                break;
            set_to_angle<Model>(controller, id, i);
            usleep(1000000);
            get_angle_pos<Model>(controller, id);
        }
}

int main()
{
    try
    {
        Usb2Dynamixel controller("/dev/ttyUSB0", B57600);
        test<ProL5430S500>(controller, 1);
    }
    catch (const errors::Error& e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}