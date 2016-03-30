#include <iostream>
#include <algorithm>
#include <dynamixel/dynamixel.hpp>
#include <cmath>
#include <boost/program_options.hpp>

#include "tools/utility.hpp"

using namespace dynamixel;

void list(Utility&)
{
}

void select_command(const std::string& command, Utility& tool)
{
    if ("list" == command)
        list(tool);
}

//void parse_input TODO: put input parsing in a dedicated function

int main(int argc, char** argv)
{
    std::string port;
    int baudrate;
    float timeout;
    std::string command;

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "produce help message")
        ("port,p", po::value<std::string>()->default_value("/dev/ttyUSB0"),
            "path to the USB to dynamixel interface.\n\n"
            "Example: --port /dev/ttyACM5")
        ("baudrate,b", po::value<unsigned>()->default_value(1000000),
            "baud rate for the communication\n\n"
            "Example: -b 115200")
        ("timeout,t", po::value<float>(&timeout)->default_value(0.02),
            "timeout for the reception of data packets")
        ("command", po::value<std::string>(), "command to be executed. "
            "Available commands are:\n"
            "\tlist\t list available actuators"
            "\tangle\t command one or more actuator to go to (a) given position(s)");
    // clang-format on

    po::positional_options_description pos_desc;
    pos_desc.add("command", 1);

    po::command_line_parser parser{argc, argv};
    parser.options(desc).positional(pos_desc);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm;
    po::store(parsed_options, vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm.count("command"))
        command = vm["command"].as<std::string>();
    if (vm.count("port"))
        port = vm["port"].as<std::string>();
    if (vm.count("baudrate"))
        baudrate = get_baudrate(vm["baudrate"].as<unsigned>());

    try {
        std::cout << "Opening serial interface" << std::endl
                  << "\tport: " << port << std::endl
                  << "\tbaudrate: " << baudrate << std::endl;
        Utility tool(port, baudrate, timeout);
        std::cout << "Serial port open." << std::endl;

        std::cout << "\nExecuting the following command" << std::endl
                  << "\tcommand: " << command << std::endl;
        select_command(command, tool);
    }
    catch (errors::Error e) {
        std::cerr << "Error (dynamixel): " << e.msg() << std::endl;
    }
    return 0;
}