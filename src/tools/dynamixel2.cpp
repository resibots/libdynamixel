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
        CommandLineUtility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1)
            : _dyn_util(name, baudrate, recv_timeout)
        {
        }

        void select_command(po::variables_map vm)
        {
            std::string command = vm["command"].as<std::string>();

            // TODO: check that the required parameters are given; otherwise display help
            if ("list" == command) {
                list();
            }
            else if ("change-id" == command) {
                change_id(vm["id"].as<std::vector<long long int>>());
            }
            else if ("change-baudrate" == command) {
                // TODO: test for Dynamixel Pro actuators
                if (vm.count("id"))
                    change_baudrate(
                        vm["id"].as<std::vector<long long int>>(),
                        vm["new-baudrate"].as<unsigned int>());
                else
                    change_baudrate(vm["new-baudrate"].as<unsigned int>());
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
            else if ("torque-enable" == command) {
                bool enable = vm["enable"].as<bool>();

                if (vm.count("id"))
                    torque_enable(
                        vm["id"].as<std::vector<long long int>>(),
                        enable);
                else
                    torque_enable(enable);
            }
            else {
                std::cout << "Unrecognized command." << std::endl;
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

        void change_id(const std::vector<long long int>& ids)
        {
            _dyn_util.detect_servos();

            if (0 == ids.size() % 2) {
                for (unsigned i = 0; i + 1 < ids.size(); i += 2) {
                    _dyn_util.change_id(ids.at(i), ids.at(i + 1));
                }
            }
            else if (1 == ids.size()) {
                _dyn_util.change_id(Protocol::broadcast_id, ids.at(0));
            }
            else {
                std::cerr << "Inproper number of parameters for change-id: expecting "
                          << "only one id or an even number of ids" << std::endl;
            }
        }

        void change_baudrate(const std::vector<long long int>& ids, unsigned int baudrate)
        {
            _dyn_util.detect_servos();

            for (auto id : ids) {
                _dyn_util.change_baudrate(id, baudrate);
            }
        }
        void change_baudrate(unsigned int baudrate)
        {
            _dyn_util.detect_servos();
            _dyn_util.change_baudrate(Protocol::broadcast_id, baudrate);
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

        void torque_enable(const std::vector<long long int>& ids, bool enable = true)
        {
            _dyn_util.detect_servos();

            for (auto id : ids) {
                _dyn_util.torque_enable(id, enable);
            }
        }

        void torque_enable(bool enable = true)
        {
            _dyn_util.detect_servos();
            _dyn_util.torque_enable(Protocol::broadcast_id, enable);
        }
    };
}

void display_help(const std::string program_name,
    const po::options_description& desc, const po::variables_map& vm)
{
    std::map<std::string, std::string> command_help;
    // clang-format off
    command_help["list"] =
        "List available actuators. Does not take any optional parameter";
    command_help["change-id"] =
        "Change the ID of one or more device(s). Requires the parameter --id.\n\n"
        "Set one ID for all connected servos\n"
        "Give the desired ID as single parmeter to --id.\n"
        "EXAMPLE: "+program_name+" change-id --id 54\n"
        "\tsets the ID 54 to all connected servos\n"
        "\n"
        "Set IDs for one or more targetted actuators\n"
        "To do so, write pairs of IDs, the first item of each pair being the "
        "original ID and the second item being the new ID.\n"
        "EXAMPLE: "+program_name+" change-id --id 1 2 7 54\n"
        "\tsets ID 2 to servo 1 and ID 54 to servo 7";
    command_help["position"] =
        "Command one or more actuator to go to (a) given position(s).\n\n"
        "As many angles as servo ids are provided\n"
        "\tEach angle is set for the corresponding servo\n"
        "\tEXAMPLE: "+program_name+" position --id 1 5 --angle 1.254 4.189\n"
        "\twill move actuator 1 to angle 1.254 rad, and 5 to 4.189 rad\n"
        "\n"
        "One angle for several ids\n"
        "\tAll listed actuators are moved to the given angle\n"
        "\tEXAMPLE: "+program_name+" position --id 1 51 24 5 --angle 3.457";
    command_help["get-position"] =
        "Retrieve current angular position of one or more servo"
        "If given ids, it will ask to the selected servos for their angular "
        "position. Otherwise, it will get it for all available servo.\n"
        "EXAMPLES:\n"
        "\t" + program_name + " get-position --id 1 54\n"
        "\tGives the positions for servos 1 and 54\n"
        "\t" + program_name + " get-position";
    //clang-format on

    // Write the command specific help message if a command is specified and it
    // is one of the supported commands
    if (vm.count("command")) {
        std::string command = vm["command"].as<std::string>();
        std::string help_message;

        if (command_help.count(command))
            help_message = command_help[command];
        else
            help_message = "Unrecognized command: " + command;

        std::cout << help_message << std::endl;
    }
    else {
        std::cout << desc << std::endl;
    }
}

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
        ("command", po::value<std::string>(), "command to be executed. Call "
            "with `--help COMMAND` to get help for one command. Available "
            "commands:\n"
            "  list\n"
            "  position\n"
            "  get-position\n"
            "  change-id\n"
            "  torque-enable")
        ("id", po::value<std::vector<long long int>>()->multitoken(),
            "one or more ids of devices")
        ("angle", po::value<std::vector<double>>()->multitoken(),
            "desired angle (in radians), used for the \"position\" command")
        ("new-baudrate", po::value<unsigned>(),
            "used by change-baudrate as the new baudrate value to be set")
        ("enable", po::value<bool>(),
            "enable (or disable) the selected servo(s)");
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
        display_help(argv[0], desc, vm);
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