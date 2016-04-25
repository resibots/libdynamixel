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

#include <stdexcept>

using namespace dynamixel;
namespace po = boost::program_options;

/**
TODO: catch exceptions !
set-angle: dynamixel::errors::ServoLimitError if angle is out of the servo's feasible positions
set-angle (vectors): runtime_error if vectors don't have same size
get-angles: runtime_error if recieved packet is not valid (checksum error or other)
**/

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

            // TODO: check that the required parameters are given; otherwise display help
            // TODO: test for Dynamixel Pro actuators
            try {
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
            catch (std::out_of_range e) {
                std::cerr << "An error (out_of_range from " << e.what()
                          << ") has been catched. You probably used a nonexistant ID."
                          << std::endl;
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

            std::cout << "Angular positions of the actuators:" << std::endl;
            for (unsigned i = 0; i < ids.size(); ++i) {
                std::cout << ids[i] << "\t" << positions[i] << std::endl;
            }
        }

        void print_position()
        {
            _dyn_util.detect_servos();

            std::pair<std::vector<id_t>, std::vector<double>> angles;
            angles = _dyn_util.get_angle();

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

#endif