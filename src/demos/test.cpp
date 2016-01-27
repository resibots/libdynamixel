#include <iostream>
#include <iomanip>
#include <dynamixel/dynamixel.hpp>
#include <cmath>

using namespace dynamixel;
using namespace models;
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

int main()
{
    try
    {
        Usb2Dynamixel controller("/dev/ttyUSB0", B1000000);

        get_model_number<Ax12>(controller, 1);
        get_model_number<Ax18>(controller, 2);
        get_model_number<Mx28>(controller, 3);
        get_model_number<Mx64>(controller, 4);
        get_model_number<Mx106>(controller, 5);

        set_max_torque<Ax12>(controller, 1, 1023);
        set_max_torque<Ax18>(controller, 2, 1023);
        set_max_torque<Mx28>(controller, 3, 1023);
        set_max_torque<Mx64>(controller, 4, 1023);
        set_max_torque<Mx106>(controller, 5, 1023);

        get_max_torque<Ax12>(controller, 1);
        get_max_torque<Ax18>(controller, 2);
        get_max_torque<Mx28>(controller, 3);
        get_max_torque<Mx64>(controller, 4);
        get_max_torque<Mx106>(controller, 5);

        set_torque_enabled<Ax12>(controller, 1);
        set_torque_enabled<Ax18>(controller, 2);
        set_torque_enabled<Mx28>(controller, 3);
        set_torque_enabled<Mx64>(controller, 4);
        set_torque_enabled<Mx106>(controller, 5);

        set_vel_to_zero<Ax12>(controller, 1);
        set_vel_to_zero<Ax18>(controller, 2);
        set_vel_to_zero<Mx28>(controller, 3);
        set_vel_to_zero<Mx64>(controller, 4);
        set_vel_to_zero<Mx106>(controller, 5);

        std::cin.get();

        set_pos_to_zero<Ax12>(controller, 1);
        set_pos_to_zero<Ax18>(controller, 2);
        set_pos_to_zero<Mx28>(controller, 3);
        set_pos_to_zero<Mx64>(controller, 4);
        set_pos_to_zero<Mx106>(controller, 5);

        std::cin.get();

        set_pos_to_max<Ax12>(controller, 1);
        set_pos_to_max<Ax18>(controller, 2);
        set_pos_to_max<Mx28>(controller, 3);
        set_pos_to_max<Mx64>(controller, 4);
        set_pos_to_max<Mx106>(controller, 5);
    }
    catch (const Error& e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}