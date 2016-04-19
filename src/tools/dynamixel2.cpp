#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread> // for a standard-compliant way to sleep

#include <boost/program_options.hpp>

#include <dynamixel/dynamixel.hpp>
#include <tools/utility.hpp>

#include <stdexcept>

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
            // TODO: test for Dynamixel Pro actuators
            if ("list" == command) {
                list();
            }
            else if ("write" == command) {
                long long int data = vm["value"].as<long long int>();
                unsigned short size = vm["size"].as<unsigned short>();
                typename Protocol::address_t address = vm["address"].as<uint16_t>();
                bool is_signed = vm.count("signed");

                if (vm.count("id")) {
                    std::vector<id_t> ids = vm["id"].as<std::vector<id_t>>();
                    write(ids, address, data, size, is_signed);
                }
                else {
                    write(address, data, size, is_signed);
                }
            }
            else if ("read" == command) {
                if (!vm.count("size"))
                    throw std::runtime_error("the --size option is needed to read data");
                unsigned short size = vm["size"].as<unsigned short>();

                if (!vm.count("address"))
                    throw std::runtime_error("the --address option is needed to read data");
                typename Protocol::address_t address = vm["address"].as<uint16_t>();

                bool is_signed = vm.count("signed");

                if (vm.count("id")) {
                    std::vector<id_t> ids = vm["id"].as<std::vector<id_t>>();
                    read(ids, address, size, is_signed);
                }
                else {
                    read(address, size, is_signed);
                }
            }
            else if ("change-id" == command) {
                change_id(vm["id"].as<std::vector<id_t>>());
            }
            else if ("change-baudrate" == command) {
                if (vm.count("id"))
                    change_baudrate(
                        vm["id"].as<std::vector<id_t>>(),
                        vm["new-baudrate"].as<unsigned int>());
                else
                    change_baudrate(vm["new-baudrate"].as<unsigned int>());
            }
            else if ("position" == command) {
                position(vm["id"].as<std::vector<id_t>>(),
                    vm["angle"].as<std::vector<double>>());
            }
            else if ("get-position" == command) {
                if (vm.count("id"))
                    print_position(vm["id"].as<std::vector<id_t>>());
                else {
                    print_position();
                }
            }
            else if ("torque-enable" == command || "relax" == command) {
                // if the relax command is used, enable is false
                // otherwise, take the value of parameter `enable`
                bool enable = "relax" != command && vm["enable"].as<bool>();

                if (vm.count("id"))
                    torque_enable(
                        vm["id"].as<std::vector<id_t>>(),
                        enable);
                else
                    torque_enable(enable);
            }
            else if ("oscillate" == command) {
                oscillate(
                    vm["angular_f"].as<float>(),
                    vm["amplitude"].as<float>(),
                    vm["offset"].as<float>(),
                    vm["phase"].as<float>(),
                    vm["duration"].as<float>());
            }
            else {
                std::cerr << "Unrecognized command." << std::endl;
            }
        }

    protected:
        typedef typename Utility<Protocol>::id_t id_t;

    private:
        Utility<Protocol> _dyn_util;

        void list()
        {
            _dyn_util.detect_servos();
            std::map<typename Protocol::id_t, std::shared_ptr<BaseServo<Protocol>>>
                actuators = _dyn_util.servos();

            std::cout << "Connected devices (" << actuators.size() << ") :"
                      << std::endl;
            for (auto actuator : actuators) {
                std::cout << (long long int)actuator.first
                          << "\t" << actuator.second->model_name() << std::endl;
            }
        }

        void write(std::vector<id_t> ids, typename Protocol::address_t address,
            long long int data, unsigned short size, bool is_signed)
        {
            if (1 == size) { // one byte of data, unsigned
                for (auto id : ids) {
                    _dyn_util.template write<uint8_t>(id, address, data);
                }
            }
            else if (2 == size) { // two bytes of data, unsigned
                for (auto id : ids) {
                    _dyn_util.template write<uint16_t>(id, address, data);
                }
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    for (auto id : ids) {
                        _dyn_util.template write<uint32_t>(id, address, data);
                    }
                }
                else {
                    for (auto id : ids) {
                        _dyn_util.template write<int32_t>(id, address, data);
                    }
                }
            }
            else {
                std::cerr << "A valid size (1, 2, 4) for the data has to be specified"
                          << std::endl;
            }
        }

        void write(typename Protocol::address_t address,
            long long int data, unsigned short size, bool is_signed)
        {
            _dyn_util.detect_servos();

            if (1 == size) { // one byte of data, unsigned
                _dyn_util.template write<uint8_t>(address, data);
            }
            else if (2 == size) { // two bytes of data, unsigned
                _dyn_util.template write<uint16_t>(address, data);
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    _dyn_util.template write<uint32_t>(address, data);
                }
                else {
                    _dyn_util.template write<int32_t>(address, data);
                }
            }
            else {
                std::cerr << "A valid size (1, 2, 4) for the data has to be specified"
                          << std::endl;
            }
        }

        void read(std::vector<id_t> ids, typename Protocol::address_t address,
            unsigned short size, bool is_signed)
        {
            if (1 == size) { // one byte of data, unsigned
                for (auto id : ids) {
                    std::cout
                        << _dyn_util.template read<uint8_t>(id, address, 1)
                        << "\n";
                }
            }
            else if (2 == size) { // two bytes of data, unsigned
                for (auto id : ids) {
                    std::cout
                        << _dyn_util.template read<uint16_t>(id, address, 2)
                        << "\n";
                }
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    for (auto id : ids) {
                        std::cout
                            << _dyn_util.template read<uint32_t>(id, address, 4)
                            << "\n";
                    }
                }
                else {
                    for (auto id : ids) {
                        std::cout
                            << _dyn_util.template read<int32_t>(id, address, 4)
                            << "\n";
                    }
                }
            }
            else {
                std::cerr << "A valid size (1, 2, 4) for the data has to be specified"
                          << std::endl;
            }
        }

        void read(typename Protocol::address_t address, unsigned short size,
            bool is_signed)
        {
            _dyn_util.detect_servos();

            if (1 == size) { // one byte of data, unsigned
                print_data(_dyn_util.template read<uint8_t>(address, 1));
            }
            else if (2 == size) { // two bytes of data, unsigned
                print_data(_dyn_util.template read<uint16_t>(address, 2));
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    print_data(_dyn_util.template read<uint32_t>(address, 4));
                }
                else {
                    print_data(_dyn_util.template read<int32_t>(address, 4));
                }
            }
            else {
                std::cerr << "A valid size (1, 2, 4) for the data has to be specified"
                          << std::endl;
            }
        }

        template <typename T>
        void print_data(std::vector<std::pair<id_t, T>> pairs)
        {
            for (auto pair : pairs)
                std::cout << pair.first << "\t" << (long long int)pair.second << "\n";
        }

        void change_id(const std::vector<id_t>& ids)
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

        void change_baudrate(const std::vector<id_t>& ids, unsigned int baudrate)
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

        void position(const std::vector<id_t>& ids, const std::vector<double>& angles)
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

        void print_position(const std::vector<id_t>& ids)
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

            std::pair<std::vector<id_t>, std::vector<double>> angles;
            angles = _dyn_util.get_angles();

            std::cout << "Angular positions of the actuators:" << std::endl;
            for (unsigned i = 0; i < angles.first.size(); ++i) {
                std::cout << angles.first[i] << "\t" << angles.second[i] << std::endl;
            }
        }

        void torque_enable(const std::vector<id_t>& ids, bool enable = true)
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

        void oscillate(
            float w,
            float A,
            float offset,
            float Phi,
            float duration = 10)
        {
            using namespace std::chrono;

            _dyn_util.detect_servos();

            steady_clock::time_point t1 = steady_clock::now();
            long long int t = 0;

            offset += M_PI;

            while (
                (t = duration_cast<milliseconds>(steady_clock::now() - t1).count())
                < duration * 1000) {

                double angle = A * sin(t / 1000.0 * w + Phi) + offset;
                _dyn_util.set_angle(angle);
                std::this_thread::sleep_for(milliseconds(10));
            }

            // move to the angle at t=0
            double angle = A * sin(Phi) + offset;
            _dyn_util.set_angle(angle);
        }

        // void oscillate(
        //     std::vector<id_t> ids,
        //     std::vector<float> w,
        //     std::vector<float> A,
        //     std::vector<float> offset,
        //     std::vector<float> Phi,
        //     float duration = 10)
        // {
        // }
    };
}

void display_help(const std::string program_name,
    const po::options_description& desc, const po::variables_map& vm)
{
    std::map<std::string, std::string> command_help;
    // clang-format off
    command_help["list"] =
        "List available actuators. Does not take any optional parameter";
    command_help["position"] =
        "Command one or more actuator to go to (a) given position(s). The angles\n"
        "are in radian.\n"
        "\n"
        "An angle for each given servo\n"
        "\tEach angle is set for the corresponding servo. There must be the same\n"
        "\tnumber of values for --id and --angle.\n"
        "\n"
        "\tEXAMPLE: "+program_name+" position --id 1 5 --angle 1.254 4.189\n"
        "\twill move actuator 1 to angle 1.254 rad, and 5 to 4.189 rad\n"
        "\n"
        "One angle for several ids\n"
        "\tAll listed actuators are moved to the given angle\n\n"
        "\tEXAMPLE: "+program_name+" position --id 1 51 24 5 --angle 3.457";
    command_help["read"] =
        "Read data in the memory of a servo.\n"
        "Requires the --address and --size options. IDs can be providen, to\n"
        "target specific servos. It also gives a much faster response.\n"
        "\n"
        "EXAMPLE: "+program_name+" read --address 36 --size 2 --id 5\n"
        "\twill display the current position of actuator 2 in ticks (for\n"
        "\tprotocol1). `size` is the number of bytes that are to be read. As the\n"
        "\tcurrent position is stored in two bytes, we use `--size 2`.";
    command_help["write"] =
        "Write some value in a servo's memory.\n"
        "Requires the --address, --size and --value options. If IDs are also\n"
        "provided, only the selected devices will be affected by the write.\n"
        "\n"
        "CAUTION: this command can have unwanted consequences, specifically if\n"
        "\tyou don't specify IDs and/or if you change fields like the baudrate\n"
        "\tor the device's ID.\n"
        "\n"
        "EXAMPLE: "+program_name+" write --address 30 --size 2 --value 25 --id 5\n"
        "\twill write the value 25 in two bytes at address 30 of servo 5.`size`\n"
        "\tis the number of bytes that are to be written.";
    command_help["get-position"] =
        "Retrieve current angular position of one or more servo, in radian.\n"
        "If given ids, it will ask to the selected servos for their angular\n"
        "position. Otherwise, it will get it for all available servo.\n"
        "\n"
        "EXAMPLES:\n"
        "\t" + program_name + " get-position --id 1 54\n"
        "\tgives the positions for servos 1 and 54\n"
        "\n"
        "\t" + program_name + " get-position\n"
        "\tgives the angular position of each connected servo";
    command_help["change-id"] =
        "Change the ID of one or more servo.\n"
        "\n"
        "For all connected servos\n"
        "\tIf only one ID is given, all connected servo will get this new ID. It\n"
        "\tshould only be used if a single servo is connected.\n"
        "\n"
        "\tEXAMPLE: "+program_name+" change-id --id 54\n"
        "\tsets the ID 54 to all connected servos\n"
        "\n"
        "For specified actuators\n"
        "\tRequires pairs of IDs, the first item of each pair being the original\n"
        "\tID and the second item being the new ID.\n"
        "\n"
        "\tEXAMPLES:\n"
        "\t"+program_name+" change-id --id 5 25\n"
        "\tset the new ID 25 to actuator 5\n"
        "\n"
        "\t"+program_name+" change-id --id 6 5 7 8 9 4\n"
        "\tdo the following changes in IDs: 6 becomes 5, 7 becomes 8, 9 becomes 4";
    command_help["change-baudrate"] =
        "Tell one or more servos to talk at a given baudrate.\n"
        "For this command to work, like any other one, it has to be sent using\n"
        "the current baudrate of the servo.\n"
        "\n"
        "For all connected servos\n"
        "\tAll connected servo will talk in the new baudrate.\n"
        "\n"
        "\tEXAMPLE: "+program_name+" change-baudrate -b 57600\n"
        "\t\t--new-baudrate 1000000\n"
        "\tmake all servos that currently use 57600 baudrate change to 1000000\n"
        "\n"
        "For specified actuators\n"
        "\tIf you give some IDs, ony these ones will be affected\n"
        "\n"
        "\tEXAMPLE: "+program_name+" change-baudrate -b 57600 --id 5 25\n"
        "\t\t--new-baudrate 1000000\n"
        "\n"
        "Allowed baudrates:\n"
        "| baud     | Protocol 1 | Protocol 2 |\n"
        "|----------|------------|------------|\n"
        "| 9600     |     Y      |     Y      |\n"
        "| 19200    |     Y      |     N      |\n"
        "| 57600    |     Y      |     Y      |\n"
        "| 115200   |     Y      |     Y      |\n"
        "| 200000   |     Y      |     N      |\n"
        "| 250000   |     Y      |     N      |\n"
        "| 400000   |     Y      |     N      |\n"
        "| 500000   |     Y      |     N      |\n"
        "| 1000000  |     Y      |     Y      |\n"
        "| 2000000  |     N      |     Y      |\n"
        "| 3000000  |     N      |     Y      |\n"
        "| 4000000  |     N      |     Y      |\n"
        "| 4500000  |     N      |     Y      |\n"
        "| 10500000 |     N      |     Y      |\n";
    command_help["torque-enable"] =
        "Enable or disable the torque on servo(s).\n"
        "\n"
        "EXAMPLES:\n"
        "\t"+program_name+" torque-enable --enable 1\n"
        "\twill affect all connected servos, enabling their output; using 0\n"
        "\tinstead of 1 would have disabled them\n"
        "\n"
        "\t"+program_name+" torque-enable --id 1 11 21 --enable 0\n"
        "\twill disable only servos 1, 11 and 21";
    command_help["relax"] =
        "Same as `torque-enable --enable 0`";
    command_help["oscillate"] =
        "Actuate all servos with a sinusoid.\n"
        "Useful options:\n"
        "\t--angular_f\n"
        "\t--amplitude\n"
        "\t--offset\n"
        "\t--phase\n"
        "\t--duration\n"
        "See the help message for more detail on the role and default value of\n"
        "each of these options.\n"
        "\n"
        "EXAMPLE: "+program_name+" oscillate --duration 20\n"
        "\twill make all connected servos turn during 20 seconds using the\n"
        "\tdefault parameters for the sinusoid";
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
        std::cout << "Usage: " + program_name + " COMMAND [options]\n\n"
            << "Available commands:\n"
            "  list\n"
            "  read\n"
            "  write\n"
            "  position\n"
            "  get-position\n"
            "  change-id\n"
            "  change-baudrate\n"
            "  torque-enable\n"
            "  relax\n"
            "  oscillate\n"
            "Use `"+program_name+" --help COMMAND` to get help for one "
            "command."
            << "\n\n"
            << desc;
    }
}

int main(int argc, char** argv)
{
    using Protocol = protocols::Protocol1;
    typedef Utility<Protocol>::id_t id_t;

    std::string port;
    int baudrate = 0, posix_baudrate = 0;
    float timeout;
    std::string command;

    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "produce help message, also for each command")
        ("port,p", po::value<std::string>()->default_value("/dev/ttyUSB0"),
            "path to the USB to dynamixel interface.\n"
            "EXAMPLE: --port /dev/ttyACM5")
        ("baudrate,b", po::value<unsigned>()->default_value(1000000),
            "baud rate for the communication\n"
            "EXAMPLE: -b 115200\n"
            "See the help for the `change-baudrate` command for the accepted "
            "values")
        ("timeout,t", po::value<float>(&timeout)->default_value(0.02),
            "timeout for the reception of data packets")
        ("id", po::value<std::vector<id_t>>()->multitoken(),
            "one or more ids of devices")
        ("angle", po::value<std::vector<double>>()->multitoken(),
            "desired angle (in radians), used for the \"position\" command")
        ("new-baudrate", po::value<unsigned>(),
            "used by change-baudrate as the new baudrate value to be set")
        ("enable", po::value<bool>(),
            "enable (1) or disable (0)) the selected servo(s)")
        ("address,a", po::value<uint16_t>(),
            "address at which to read or write data, in decimal notation")
        ("value,d", po::value<long long int>(),
            "the value to write when using the `write` command")
        ("size", po::value<unsigned short>(),
            "number of bytes of data either read or written from a servo")
        ("signed",
            "when reading a field, whether it should be parsed as signed (only "
            "for protocol 2)")
        ("angular_f,w", po::value<float>()->default_value(2*M_PI),
            "angular frequency used for the oscillate command (in rad/s)")
        ("amplitude,A", po::value<float>()->default_value(1),
            "peak amplitude of the movement performed by the oscillate command (in rad)")
        ("offset,B", po::value<float>()->default_value(0),
            "offset (in rad) of the angle in oscillate command")
        ("phase,P", po::value<float>()->default_value(0),
            "phase shift for the oscillate command")
        ("duration,d", po::value<float>()->default_value(10.0),
            "duration, in seconds of the oscillation (can be float)");
    // clang-format on

    po::options_description hidden("Hidden options");
    // clang-format off
    hidden.add_options()
        ("command", po::value<std::string>(), "command executed");
    // clang-format on

    po::options_description cmdline_options;
    cmdline_options.add(desc).add(hidden);

    po::positional_options_description pos_desc;
    pos_desc.add("command", 1);

    po::command_line_parser parser{argc, argv};
    parser.options(cmdline_options).positional(pos_desc);
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
        display_help(argv[0], desc, vm);
        return 1;
    }
    if (vm.count("port"))
        port = vm["port"].as<std::string>();
    if (vm.count("baudrate")) {
        baudrate = vm["baudrate"].as<unsigned>();
        posix_baudrate = get_baudrate(baudrate);
    }

    try {
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