#include <iostream>
#include <iomanip>
#include <dynamixel/dynamixel.hpp>
#include <cmath>

using namespace dynamixel;

int main()
{
    size_t size = 0;
    Ping<Protocol1> ping1(0);
    size = ping1.packet().size();

    std::cout << "Ping Protcol 1: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)ping1.packet()[i] << " ";
    std::cout << std::endl;

    Ping<Protocol2> ping2(0);
    size = ping2.packet().size();

    std::cout << "Ping Protcol 2: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)ping2.packet()[i] << " ";
    std::cout << std::endl;

    Read<Protocol1> read1(1, 0x2B, 0x01);
    size = read1.packet().size();

    std::cout << "Read Protcol 1: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)read1.packet()[i] << " ";
    std::cout << std::endl;

    Read<Protocol2> read2(1, 0x2B, 0x01);
    size = read2.packet().size();

    std::cout << "Read Protcol 2: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)read2.packet()[i] << " ";
    std::cout << std::endl;

    std::vector<uint8_t> data(1);
    data[0] = 0x01;
    Write<Protocol1> write1(0, 0x19, data);
    size = write1.packet().size();

    std::cout << "Write Protcol 1: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)write1.packet()[i] << " ";
    std::cout << std::endl;

    data[0] = 0xFF;
    Write<Protocol2> write2(1, 563, data);   
    size = write2.packet().size();

    std::cout << "Write Protcol 2: ";
    for (size_t i = 0; i < size; i++)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)write2.packet()[i] << " ";
    std::cout << std::endl;
     
    Usb2Dynamixel controller("/dev/ttyUSB0", B57600);
    controller.send(write2);

    return 0;
}