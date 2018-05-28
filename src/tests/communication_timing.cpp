#include "../src/dynamixel/dynamixel.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << "port_name recv_timeout"
                  << std::endl;
        return 1;
    }
    double recv_timeout = atof(argv[2]);
    dynamixel::StatusPacket<dynamixel::protocols::Protocol1> status;

    try {
        dynamixel::controllers::Usb2Dynamixel io(argv[1], 1000000, recv_timeout);
        bool success = false;

        while (true) {
            auto send_start = std::chrono::system_clock::now();
            io.send(dynamixel::servos::Mx28::get_present_position_angle(1));
            auto send_end = std::chrono::system_clock::now();

            auto recv_start = std::chrono::system_clock::now();
            success = io.recv(status);
            auto recv_end = std::chrono::system_clock::now();

            std::cout << "present position of servo 1: "
                      //   << dynamixel::servos::Mx28::parse_present_position_angle(1, status)
                      << std::endl;
            std::chrono::duration<double> send_dt = send_end - send_start;
            std::chrono::duration<double> recv_dt = recv_end - recv_start;
            std::cout << "sending time: " << send_dt.count() << "s" << std::endl
                      << "receiveing time: " << recv_dt.count() << "s"
                      << std::endl;
            std::cout << "receive was " << (success ? "successful" : "failed")
                      << "\n" << std::endl;
        }
    }
    catch (dynamixel::errors::Error& e) {
        std::cerr << e << std::endl;
    }
}