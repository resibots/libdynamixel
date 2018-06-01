#include "../src/dynamixel/dynamixel.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " port_name recv_timeout"
                  << std::endl;
        return 1;
    }
    double recv_timeout = atof(argv[2]);
    dynamixel::StatusPacket<dynamixel::protocols::Protocol1> status;

    try {
        // dynamixel::controllers::Usb2Dynamixel io(argv[1], B57600, recv_timeout);
        dynamixel::controllers::Usb2Dynamixel io(argv[1], B1000000, recv_timeout);
        bool success = false;

        std::chrono::time_point<std::chrono::system_clock>
            send_start, send_end, recv_start, recv_end;

        while (true) {
            send_start = std::chrono::system_clock::now();
            io.send(dynamixel::servos::Mx28::get_present_position_angle(1));
            send_end = std::chrono::system_clock::now();

            // std::this_thread::sleep_for(std::chrono::microseconds(20000));

            recv_start = std::chrono::system_clock::now();
            // ===============================================================
            // FIXME: Maybe we should read as many bytes as relevant at once ?
            // ===============================================================
            // success = io.recv(status);
            success = io.recv_multibyte(status);
            recv_end = std::chrono::system_clock::now();

            std::cout << "present position of servo 1: "
                      //   << dynamixel::servos::Mx28::parse_present_position_angle(1, status)
                      << std::endl;
            std::chrono::duration<double> send_dt = send_end - send_start;
            std::chrono::duration<double> recv_dt = recv_end - recv_start;
            std::chrono::duration<double> reply_dt = recv_end - send_end;
            std::cout << "sending time: " << send_dt.count() << "s" << std::endl
                      << "receiveing time: " << recv_dt.count() << "s" << std::endl
                      << "reply time: " << reply_dt.count() << "s"
                      << std::endl;
            std::cout << "receive has " << (success ? "succeeded" : "failed")
                      << "\n"
                      << std::endl;
        }
    }
    catch (dynamixel::errors::Error& e) {
        std::cerr << e << std::endl;
    }
}