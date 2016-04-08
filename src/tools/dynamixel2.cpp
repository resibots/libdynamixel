#include <iostream>
#include <string>
#include <algorithm>
#include <dynamixel/dynamixel.hpp>
#include <cmath>
#include <boost/program_options.hpp>

#include "tools/utility.hpp"

using namespace dynamixel;
namespace po = boost::program_options;

// TODO: catch exceptions !

namespace dynamixel {
    template <class Protocol>
    class CommandLineUtility {
    public:
        CommandLineUtility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1) : _dyn_util(name, baudrate, recv_timeout)
        {
        }

        void select_command(po::variables_map vm)
        {
            std::string command = vm["command"].as<std::string>();

            if ("list" == command) {
                list();
            }
            else if ("position" == command) {
                position(vm["id"].as<std::vector<long long int>>(),
                    vm["angle"].as<std::vector<double>>());
            }
            else if ("get-position" == command) {
                if (vm.count("id"))
                    print_position(vm["id"].as<std::vector<long long int>>());
                else {
                    print_position();
                }
            }
        }

    private:
        Utility<Protocol> _dyn_util;

        void list()
        {
            _dyn_util.detect_servos();
            std::map<typename Protocol::address_t, std::shared_ptr<BaseServo<Protocol>>>
                actuators = _dyn_util.servos();

            std::cout << "Connected devices (" << actuators.size() << ") :"
                      << std::endl;
            for (auto actuator : actuators) {
                std::cout << (long long int)actuator.first
                          << "\t" << actuator.second->model_name() << std::endl;
            }
        }

        void position(const std::vector<long long int>& ids, const std::vector<double>& angles)
        {
            if (angles.size() == 1) {
                _dyn_util.detect_servos();
                _dyn_util.set_angle(ids, angles.at(0));
            }
            else if (ids.size() == angles.size()) {
                _dyn_util.detect_servos();
                _dyn_util.set_angle(ids, angles);
            }
            else
                std::cout << "Usage for position command: " << std::endl
                          << "- one angle and several ids of servos that all "
                             "wil go to the same angle"
                          << std::endl
                          << "- as many angles as there are ids, to give target "
                             "angle for each servo"
                          << std::endl;
            // TODO: manage limit cases
        }

        void print_position(const std::vector<long long int>& ids)
        {
            if (ids.size() == 0)
                return;

            _dyn_util.detect_servos();

            std::vector<double> positions;
            positions = _dyn_util.get_angles(ids);

            std::cout << "Angular positions of the actuators:" << std::endl;
            for (unsigned i = 0; i < ids.size(); ++i) {
                std::cout << ids[i] << "\t" << positions[i] << std::endl;
            }
        }

        void print_position()
        {
            _dyn_util.detect_servos();

            std::pair<std::vector<long long int>, std::vector<double>> angles;
            angles = _dyn_util.get_angles();

            std::cout << "Angular positions of the actuators:" << std::endl;
            for (unsigned i = 0; i < angles.first.size(); ++i) {
                std::cout << angles.first[i] << "\t" << angles.second[i] << std::endl;
            }
        }
    };
}

//void parse_input TODO: put input parsing in a dedicated function

// TODO: show help message for each command if called with `--help command`

int main(int argc, char** argv)
{
    std::string port;
    int baudrate = 0, posix_baudrate = 0;
    float timeout;
    std::string command;

    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "produce help message")
        ("port,p", po::value<std::string>()->default_value("/dev/ttyUSB0"),
            "path to the USB to dynamixel interface.\n"
            "Example: --port /dev/ttyACM5")
        ("baudrate,b", po::value<unsigned>()->default_value(1000000),
            "baud rate for the communication\n"
            "Example: -b 115200")
        ("timeout,t", po::value<float>(&timeout)->default_value(0.02),
            "timeout for the reception of data packets")
        ("id", po::value<std::vector<long long int>>()->multitoken(),
            "one or more ids of devices")
        ("angle", po::value<std::vector<double>>()->multitoken(),
            "desired angle, used for the \"position\" command")
        ("command", po::value<std::string>(), "command to be executed. "
            "Available commands are:\n"
            "  list     list available actuators"
            "  position command one or more actuator to go to (a) given position(s)"
            "  get-position retrieve current angular position of one or more servo");
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
    else {
        std::cerr << "No command given to the software!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    if (vm.count("port"))
        port = vm["port"].as<std::string>();
    if (vm.count("baudrate")) {
        baudrate = vm["baudrate"].as<unsigned>();
        posix_baudrate = get_baudrate(baudrate);
    }

    try {
        using Protocol = protocols::Protocol1;

        std::cout << "Opening serial interface" << std::endl
                  << "\tport: " << port << std::endl
                  << "\tbaudrate: " << baudrate << std::endl;
        CommandLineUtility<Protocol> command_line(port, posix_baudrate, timeout);
        std::cout << "Serial port open." << std::endl;

        std::cout << "\nExecuting the following command" << std::endl
                  << "\tcommand: " << command << std::endl;
        command_line.select_command(vm);
    }
    catch (errors::Error e) {
        std::cerr << "Error (dynamixel): " << e.msg() << std::endl;
    }
    return 0;
}