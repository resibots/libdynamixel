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


template <typename Model>
void find_devices(int baudrate)
{
     Usb2Dynamixel controller("/dev/ttyUSB0", baudrate);
        StatusPacket<typename Model::protocol_t > status;

        for (int i = 0; i < 254; i++) {
            controller.send(typename Model::ping_t(i));
            if (controller.recv(RECV_TIME, status) && status.valid() && status.id() == i) {
                std::cout << "Found device with baudrate " << baudrate << " and id " << i << std::endl;
                controller.send(Model::set_led(i, true));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_torque_enabled(i, true));
                controller.recv(RECV_TIME, status);

                ids[0] = i;
                speeds[0] = 1024;

                controller.send(Model::set_speeds(ids, speeds));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_position(i, -180684));
                controller.recv(RECV_TIME, status);
                std::cin.get();

                controller.send(Model::set_torque_enabled(i, false));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_id(i, i +10));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_torque_enabled(i + 10, true));
                controller.recv(RECV_TIME, status);

                controller.send(Model::set_speed(i + 10, 0));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_position(i + 10, 0));
                controller.recv(RECV_TIME, status);
                std::cin.get();

                controller.send(Model::set_torque_enabled(i + 10, false));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_id(i + 10, i));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_torque_enabled(i, true));
                controller.recv(RECV_TIME, status);

                controller.send(Model::set_position(i, 180684));
                controller.recv(RECV_TIME, status);
                std::cin.get();
                
                controller.send(Model::set_led(i, false));
                controller.recv(RECV_TIME, status);
                controller.send(Model::set_position(i, 0));
                controller.recv(RECV_TIME, status);
                break;
            }
        }
}

int main()
{
    try
    {
        //find_devices<Mx64>(B1000000);
        find_devices<ProL54>(B57600);
        return 0;

        size_t size = 0;
        Usb2Dynamixel controller("/dev/ttyUSB0", B1000000);
        StatusPacket<Protocol1> status;

        controller.send(Mx64::ping_t(16));

        std::vector<uint8_t> data(1);
        data[0] = 255;

        //Usb2Dynamixel controller("/dev/ttyUSB0", B57600);
        //controller.send(Write<Protocol2>(1, 563, data));

        ids[0] = 16;
        positions[0] = 0;
        ids[1] = 23;
        positions[1] = 1024;
        ids[2] = 26;
        positions[2] = 2048;

        controller.send(Mx64::set_positions(ids, positions));
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        std::cin.get();

        positions[0] = 4095;
        positions[1] = 4095;
        positions[2] = 4095;

        controller.send(Mx64::set_positions(ids, positions));
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        /*controller.send(Mx64::set_position(0, 0));
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        controller.send(Mx64::led_off(0));
        controller.recv(0.1, status);
        std::cout << status << std::endl;*/
    }
    catch (const Error& e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}