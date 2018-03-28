
#include <dynamixel/dynamixel.hpp>
#include <dynamixel/operating_mode.hpp>

#include <string.h>
#include <string>
#include <iostream>

using namespace dynamixel;
using namespace servos;
using namespace controllers;
using namespace protocols;

template <class Protocol>
std::string string_operating_mode(Usb2Dynamixel& controller, typename Protocol::id_t id)
{
    OperatingMode mode = operating_mode<Protocol>(controller, id);

    std::string str_mode;
    switch (mode) {
    case OperatingMode::wheel:
        str_mode = "wheel or speed";
        break;
    case OperatingMode::joint:
        str_mode = "joint or position";
        break;
    case OperatingMode::multi_turn:
        str_mode = "multi-turn";
        break;
    default:
        str_mode = "unknown";
    }
    return str_mode;
}

int main(int argc, char** argv)
{
    if (argc != 3 || (argc == 2 && 0 == strcmp(argv[1], "--help"))) {
        std::cout << "Usage: " << argv[0] << " device protocol_number" << std::endl;
        std::cout << "This program will retrieve and print the operating mode "
                  << "(wheel or joint) of actuator with given ID." << std::endl;
        return -1;
    }

    try {
        // Default baudrate for the dynamixel pro is 57600, you might have to
        // change the line below
        Usb2Dynamixel controller(argv[1], B1000000, 0.05);

        int protocol_version = atoi(argv[2]);
        std::cout << "Protocol version " << protocol_version << std::endl;
        if (1 == protocol_version) {
            std::vector<std::shared_ptr<BaseServo<Protocol1>>> servos_1 = auto_detect<Protocol1>(controller);

            if (servos_1.size()) {
                for (auto servo : servos_1) {
                    std::string str_mode = string_operating_mode<Protocol1>(controller, servo->id());
                    std::cout << "Detected an " << servo->model_name()
                              << " with ID " << servo->id()
                              << ", in " << str_mode << " mode" << std::endl;
                }
            }
            else {
                std::cout << "No dynamixel detected" << std::endl;
            }
        }
        else if (2 == protocol_version) {
            std::vector<std::shared_ptr<BaseServo<Protocol2>>> servos_2 = auto_detect<Protocol2>(controller);

            if (servos_2.size()) {
                for (auto servo : servos_2) {
                    std::string str_mode = string_operating_mode<Protocol2>(controller, servo->id());
                    std::cout << "Detected an " << servo->model_name()
                              << " with ID " << servo->id()
                              << ", in " << str_mode << " mode" << std::endl;
                }
            }
            else {
                std::cout << "No dynamixel detected" << std::endl;
            }
        }
        else {
            std::cout << "Only versions 1 and 2 of the protocol are supported (and there are only two versions)." << std::endl;
        }
    }
    catch (const errors::Error& e) {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}