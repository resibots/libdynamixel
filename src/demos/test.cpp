#include <iostream>
#include <iomanip>
#include <dynamixel/dynamixel.hpp>
#include <cmath>

using namespace dynamixel;

int main()
{
    try
    {
        size_t size = 0;
        Ping<Protocol1> ping1(0);
        Usb2Dynamixel controller("/dev/ttyUSB0", B1000000);
        controller.send(Mx64::ping(0));

        std::vector<uint8_t> data(1);
        data[0] = 255;

        //Usb2Dynamixel controller("/dev/ttyUSB0", B57600);
        //controller.send(Write<Protocol2>(1, 563, data));

        StatusPacket<Protocol1> status;
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        controller.send(Mx64::led_on(0));
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        usleep(200000);

        controller.send(Mx64::led_off(0));
        controller.recv(0.1, status);
        std::cout << status << std::endl;

        usleep(200000);

        controller.send(Mx64::led_on(0));
        controller.recv(0.1, status);
        std::cout << status << std::endl;
    }
    catch (const Error& e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}