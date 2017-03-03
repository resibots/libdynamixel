#include <vector>
#include <sys/types.h>

#include "../dynamixel/controllers/file2dynamixel.hpp"

using namespace dynamixel;
using namespace protocols;
using namespace controllers;

void test_unpack_status_1();
void test_unpack_status_2();

int main()
{
    // test_unpack_status_1();
    test_unpack_status_2();
    return 0;
}

void test_unpack_status_1()
{
    try {
        std::cout << "writing packet to file frames.dat" << std::endl;

        // open the file with options respectively read/write, create if
        // file does not exit, truncate if file exists already
        // the file's mode is rw-rw-rw-
        int flags = (O_WRONLY | O_CREAT | O_TRUNC);
        File2Dynamixel interface("frames.dat", flags);

        // Good packet followed by the half of a packet header
        std::vector<uint8_t> packet = {0xFF, 0xFF, 0x01, 0x03, 0x00, 0x20, 0xDB, 0xFF};
        interface.send(packet);

        packet = {0xFF, 0xFF, 0x00, 0x02, 0x00, 0xFD};
        interface.send(packet);

        // A valid packet prepended with rubbish data
        packet = {0xFD, 0x00, 0x25, 0xFF, 0xDD, 0x00, 0xFF, 0xFF, 0x00, 0x02, 0x00, 0xFD};
        interface.send(packet);

        // a valid packet, except for the checksum
        packet = {0xFF, 0xFF, 0x00, 0x02, 0x00, 0xF0};
        interface.send(packet);

        // An error is reported by actuator 4 (over-heating and over-load)
        packet = {0xFF, 0xFF, 0x04, 0x02, 0x24, 0xD5};
        interface.send(packet);

        interface.close_file();

        interface.set_report_bad_packet(false);

        // Reading from the file
        StatusPacket<Protocol1>
            status;
        flags = O_RDONLY;
        interface.open_file("frames.dat", flags);
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
    }
    catch (dynamixel::errors::BadPacket e) {
        std::cout << "Catched exception\n\t" << e << std::endl;
    }
    catch (dynamixel::errors::Error e) {
        std::cout << "Catched exception\n\t" << e.msg() << std::endl;
    }
}

void test_unpack_status_2()
{
    try {
        std::cout << "writing packet to file frames.dat" << std::endl;

        // open the file with options respectively read/write, create if
        // file does not exit, truncate if file exists already
        // the file's mode is rw-rw-rw-
        int flags = (O_WRONLY | O_CREAT | O_TRUNC);
        File2Dynamixel interface("frames.dat", flags);

        // Good packet followed by the half of a packet header
        std::vector<uint8_t> packet = {0xFF, 0xFF, 0xFD, 0x00, 0x01, 0x07, 0x00, 0x55, 0x00, 0x06, 0x04, 0x26, 0x65, 0x5D, 0xFF};
        interface.send(packet);

        packet = {0xFF, 0xFF, 0xFD, 0x00, 0x01, 0x07, 0x00, 0x55, 0x00, 0x06, 0x04, 0x26, 0x65, 0x5D};
        interface.send(packet);

        // A valid packet prepended with rubbish data
        packet = {0xFD, 0x00, 0x25, 0xFF, 0xFF, 0xFD, 0x00, 0x01, 0x07, 0x00, 0x55, 0x00, 0x06, 0x04, 0x26, 0x65, 0x5D};
        interface.send(packet);

        // a valid packet, except for the checksum
        packet = {0xFF, 0xFF, 0xFD, 0x00, 0x01, 0x07, 0x00, 0x55, 0x00, 0x06, 0x04, 0x26, 0x65, 0x00};
        interface.send(packet);

        // An error is reported by actuator 4 (harware error and result fail)
        packet = {0xFF, 0xFF, 0xFD, 0x00, 0x04, 0x08, 0x00, 0x55, 0x84, 0xA6, 0x00, 0x00, 0x00, 0x8C, 0xE2};
        interface.send(packet);

        interface.close_file();

        interface.set_report_bad_packet(false);

        // Reading from the file
        StatusPacket<Protocol2>
            status;
        flags = O_RDONLY;
        interface.open_file("frames.dat", flags);
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
        std::cout << status << std::endl;
        interface.recv(status);
        std::cout << status << std::endl;
    }
    catch (dynamixel::errors::BadPacket e) {
        std::cout << "Catched exception\n\t" << e << std::endl;
    }
    catch (dynamixel::errors::Error e) {
        std::cout << "Catched exception\n\t" << e << std::endl;
    }
}