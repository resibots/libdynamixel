#ifndef COMMAND_LINE_UTILITY
#define COMMAND_LINE_UTILITY

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
#include <tools/missing_parameter_error.hpp> // an exception class

#include <stdexcept>

using namespace dynamixel;
namespace po = boost::program_options;

namespace dynamixel {
    template <class Protocol>
    class CommandLineUtility {
    public:
        /**
            @throws dynamixel::errors:Error if an issue was met while attempting
            to open the serial interface
        **/
        CommandLineUtility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1)
            : _dyn_util(name, baudrate, recv_timeout)
        {
        }

        void select_command(po::variables_map vm)
        {
            std::string command = vm["command"].as<std::string>();

            try {
                if ("list" == command) {
                    list();
                }
                else if ("write" == command) {
                    std::vector<std::string>
                        parameters = {"value", "size", "address"};
                    check_vm(vm, parameters);

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
                    std::vector<std::string> parameters = {"size", "address"};
                    check_vm(vm, parameters);

                    unsigned short size = vm["size"].as<unsigned short>();
                    typename Protocol::address_t address
                        = vm["address"].as<uint16_t>();

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
                    check_vm(vm, "id");
                    change_id(vm["id"].as<std::vector<id_t>>());
                }
                else if ("change-baudrate" == command) {
                    check_vm(vm, "new-baudrate");

                    if (vm.count("id"))
                        change_baudrate(
                            vm["id"].as<std::vector<id_t>>(),
                            vm["new-baudrate"].as<unsigned int>());
                    else
                        change_baudrate(vm["new-baudrate"].as<unsigned int>());
                }
                else if ("position" == command) {
                    check_vm(vm, "angle");

                    if (vm.count("id"))
                        position(vm["id"].as<std::vector<id_t>>(),
                            vm["angle"].as<std::vector<double>>());
                    else
                        position(vm["angle"].as<std::vector<double>>());
                }
                else if ("get-position" == command) {
                    if (vm.count("id"))
                        print_position(vm["id"].as<std::vector<id_t>>());
                    else {
                        print_position();
                    }
                }
                else if ("set-speed" == command) {
                    check_vm(vm, "speed");

                    bool wheel_mode = false;
                    if (vm.count("wheel-mode"))
                        wheel_mode = true;

                    if (vm.count("id"))
                        speed(vm["id"].as<std::vector<id_t>>(),
                            vm["speed"].as<std::vector<double>>(),
                            wheel_mode);
                    else
                        speed(vm["speed"].as<std::vector<double>>(),
                            wheel_mode);
                }
                else if ("get-speed" == command) {
                    if (vm.count("id"))
                        print_speed(vm["id"].as<std::vector<id_t>>());
                    else
                        print_speed();
                }
                else if ("torque-enable" == command || "relax" == command) {
                    check_vm(vm, "enable");
                    bool enable = vm["enable"].as<bool>();

                    if (vm.count("id"))
                        torque_enable(vm["id"].as<std::vector<id_t>>(), enable);
                    else
                        torque_enable(enable);
                }
                else if ("get-torque-enable" == command) {
                    if (vm.count("id"))
                        print_torque_enable(vm["id"].as<std::vector<id_t>>());
                    else
                        print_torque_enable();
                }
                else if ("relax" == command) {
                    if (vm.count("id"))
                        torque_enable(vm["id"].as<std::vector<id_t>>(), false);
                    else
                        torque_enable(false);
                }
                else if ("oscillate" == command) {
                    if (vm.count("id"))
                        oscillate(
                            vm["angular_f"].as<float>(),
                            vm["amplitude"].as<float>(),
                            vm["offset"].as<float>(),
                            vm["phase"].as<float>(),
                            vm["duration"].as<float>(),
                            vm["id"].as<std::vector<id_t>>());
                    else
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
            catch (std::out_of_range e) {
                std::cerr << "An error (out_of_range from " << e.what()
                          << ") has been catched. You probably used a nonexistant ID."
                          << std::endl;
            }
            catch (errors::ServoLimitError e) {
                std::cerr << e.msg() << std::endl;
            }
            catch (errors::UnpackError e) {
                std::cerr << "When attempting to read some data on a servo, we "
                          << "got the following error:\n\t"
                          << e.msg() << std::endl;
            }
            catch (errors::UtilityError e) {
                std::cerr << "Our bad! The following error is our fault :\n"
                          << e.msg() << "\n\n"
                          << "Please report this issue to the developpers." << std::endl;
            }
            catch (errors::MissingParameter e) {
                std::cerr << e.msg()
                          << "Use \"--help " << command << "\" for more information."
                          << std::endl;
            }
            catch (errors::Error e) {
                std::cerr << "Dynamixel error:\n\t" << e.msg() << std::endl;
            }
        }

    protected:
        typedef typename Utility<Protocol>::id_t id_t;

        /* Search the the required parameters in the boost:program_option's variable map.

           The missing parameters are reported to the user with an exception

           @param variables_map  built by boost::program_option, from the parsed command line arguments
           @param parameters vector of the names of the required parameters
        */
        void check_vm(const po::variables_map& vm,
            const std::vector<std::string>& parameters)
        {
            bool missing = false;
            std::vector<std::string> missing_parameters;

            // Check if each required parameter is there and store the missing ones
            for (auto parameter : parameters) {
                if (vm[parameter].empty()) {
                    missing = true;
                    missing_parameters.push_back(parameter);
                }
            }

            // If there are missing parameters, throw an exception to tell the user
            if (missing)
                throw errors::MissingParameter(vm["command"].as<std::string>(),
                    missing_parameters);
        }

        /* Search the the required parameter in the boost:program_option's variable map.

          This version is for a single required parameter.
        */
        inline void check_vm(const po::variables_map& vm, std::string parameter)
        {
            std::vector<std::string> parameters = {parameter};
            check_vm(vm, parameters);
        }

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
                        << (unsigned short)_dyn_util.template read<uint8_t>(
                               id, address)
                        << "\n";
                }
            }
            else if (2 == size) { // two bytes of data, unsigned
                for (auto id : ids) {
                    std::cout
                        << _dyn_util.template read<uint16_t>(id, address)
                        << "\n";
                }
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    for (auto id : ids) {
                        std::cout
                            << _dyn_util.template read<uint32_t>(id, address)
                            << "\n";
                    }
                }
                else {
                    for (auto id : ids) {
                        std::cout
                            << _dyn_util.template read<int32_t>(id, address)
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
                print_data(_dyn_util.template read<uint8_t>(address));
            }
            else if (2 == size) { // two bytes of data, unsigned
                print_data(_dyn_util.template read<uint16_t>(address));
            }
            else if (4 == size) { // four bytes of data, both unsigned and signed
                if (is_signed) {
                    print_data(_dyn_util.template read<uint32_t>(address));
                }
                else {
                    print_data(_dyn_util.template read<int32_t>(address));
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
                std::cout << "Usage for position command (with IDs):\n"
                             "- one angle and several ids of servos that all "
                             "wil go to the same angle\n"
                             "- as many angles as there are ids, to give target "
                             "angle for each servo"
                          << std::endl;
        }

        void position(const std::vector<double>& angles)
        {
            if (angles.size() == 1) {
                _dyn_util.detect_servos();
                _dyn_util.set_angle(angles.at(0));
            }
            else
                std::cout << "Usage for position command with no ID:\n"
                             "only one angle is accepted, and applied to all "
                             "connected servos"
                          << std::endl;
        }

        void print_position(const std::vector<id_t>& ids)
        {
            if (ids.size() == 0)
                return;

            _dyn_util.detect_servos();

            std::vector<double> positions;
            positions = _dyn_util.get_angle(ids);

            std::cout << "Angular positions of the actuators (rad):" << std::endl;
            for (unsigned i = 0; i < ids.size(); ++i) {
                std::cout << ids[i] << "\t" << positions[i] << std::endl;
            }
        }

        void print_position()
        {
            _dyn_util.detect_servos();

            std::pair<std::vector<id_t>, std::vector<double>> angles;
            angles = _dyn_util.get_angle();

            std::cout << "Angular positions of the actuators (rad):" << std::endl;
            for (unsigned i = 0; i < angles.first.size(); ++i) {
                std::cout << angles.first[i] << "\t" << angles.second[i] << std::endl;
            }
        }

        void speed(const std::vector<id_t>& ids, const std::vector<double>& speeds,
            bool wheel_mode = false)
        {
            if (speeds.size() == 1) {
                _dyn_util.detect_servos();
                _dyn_util.set_speed(ids, speeds.at(0), wheel_mode);
            }
            else if (ids.size() == speeds.size()) {
                _dyn_util.detect_servos();
                _dyn_util.set_speed(ids, speeds, wheel_mode);
            }
            else
                std::cout << "Usage for set-speed command (with IDs):\n"
                             "- one speed and several ids of servos that all "
                             "wil go to at the same angular velocity\n"
                             "- as many speeds as there are ids, to give target "
                             "speed for each servo"
                          << std::endl;
        }

        void speed(const std::vector<double>& speeds, bool wheel_mode = false)
        {
            if (speeds.size() == 1) {
                _dyn_util.detect_servos();
                _dyn_util.set_speed(speeds.at(0), wheel_mode);
            }
            else
                std::cout << "Usage for set-speed command with no ID:\n"
                             "only one speed is accepted, and applied to all "
                             "connected servos"
                          << std::endl;
        }

        void print_speed(const std::vector<id_t>& ids)
        {
            if (ids.size() == 0)
                return;

            _dyn_util.detect_servos();

            std::vector<double> speeds;
            speeds = _dyn_util.get_speed(ids);

            std::cout << "Goal angular velocities of the actuators (rad/s):" << std::endl;
            for (unsigned i = 0; i < ids.size(); ++i) {
                std::cout << ids[i] << "\t" << speeds[i] << std::endl;
            }
        }

        void print_speed()
        {
            _dyn_util.detect_servos();

            std::pair<std::vector<id_t>, std::vector<double>> speeds
                = _dyn_util.get_speed();

            std::cout << "Goal angular velocities of the actuators (rad/s):"
                      << std::endl;
            for (uint8_t i = 0; i < speeds.first.size(); ++i) {
                std::cout << speeds.first[i] << "\t" << speeds.second[i] << std::endl;
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

        void print_torque_enable(const std::vector<id_t>& ids)
        {
            _dyn_util.detect_servos();
            std::vector<bool> enabled = _dyn_util.get_torque_enable(ids);

            if (ids.size() == 1) {
                std::cout << "Actuator " << ids[0] << " is "
                          << (enabled[0] ? "enabled" : "disabled") << std::endl;
            }
            else {
                std::cout << "Enable status for the actuators (1: enabled / 0: disabled):"
                          << std::endl;
                // for each selected servo
                for (uint8_t i = 0; i < ids.size(); ++i) {
                    std::cout << (unsigned int)ids[i] << "\t"
                              << enabled[i] << std::endl;
                }
            }
        }

        void
        print_torque_enable()
        {
            _dyn_util.detect_servos();
            std::pair<std::vector<id_t>, std::vector<bool>> response
                = _dyn_util.get_torque_enable();

            std::cout << "Enable status for the actuators (1: enabled / 0: disabled):"
                      << std::endl;
            // for each servo that responded
            for (uint8_t i = 0; i < response.first.size(); ++i) {
                std::cout << (unsigned int)response.first[i] << "\t"
                          << response.second[i] << std::endl;
            }
        }

        void oscillate(
            float w,
            float A,
            float offset,
            float Phi,
            float duration = 10,
            std::vector<id_t> ids = std::vector<id_t>())
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
                if (ids.empty())
                    _dyn_util.set_angle(angle);
                else
                    _dyn_util.set_angle(ids, angle);
                std::this_thread::sleep_for(milliseconds(10));
            }

            // move to the angle at t=0
            double angle = A * sin(Phi) + offset;
            if (ids.empty())
                _dyn_util.set_angle(angle);
            else
                _dyn_util.set_angle(ids, angle);
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

#endif