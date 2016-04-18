#ifndef DYNAMIXEL_UTILITY
#define DYNAMIXEL_UTILITY

#include <dynamixel/dynamixel.hpp>

#include <map>
#include <utility> // std::pair
#include <sstream>
#include <stdexcept>

using namespace dynamixel;
using namespace controllers;
using namespace servos;
using namespace instructions;
using namespace protocols;

namespace dynamixel {
    template <class Protocol>
    class Utility {
    public:
        // We should hypothetically rely on the type Protocol::id_t but it
        // confuses boost::program_options because Protocol::id_t is resolved as
        // char and boost::program_options gets the ascii code of the digits
        // instead of the number itself.
        // Therefore, we define this type.
        typedef unsigned short id_t;

        Utility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1)
            : _serial_interface(name, baudrate, recv_timeout), _scanned(false)
        {
        }

        /** Detect the connected servos on the bus.
            These servos are then stored internally and you can use the other
            methods to send them orders or get data about them.

            Note that it will only find the servos connected to a given serial
            interface and talking at a given baudrate. Also, they have to speak
            the same protocol you use to talk to them.

            @param scan_timeout (default 0.01) set a special listening timeout;
                will only apply for this detection process
        **/
        void detect_servos(double scan_timeout = 0.01)
        {
            double original_timeout = _serial_interface.recv_timeout();
            _serial_interface.set_recv_timeout(scan_timeout);

            _servos = auto_detect_map<Protocol>(_serial_interface);
            _scanned = true;

            _serial_interface.set_recv_timeout(original_timeout);
        }

        /** Return the connected actuators.
            If we didn't do a scanning yet, does it with the default recieve
            timeout.

            @return map of ids and (std) shared pointers to BaseServo instances
        **/
        const std::map<typename Protocol::id_t, std::shared_ptr<BaseServo<Protocol>>>&
        servos() const
        {
            if (!_scanned)
                throw std::runtime_error("You need to scan for connected "
                                         "actuators before trying to retrieve them");

            return _servos;
        }

        void write(typename Protocol::id_t id, typename Protocol::address_t address,
            const std::vector<uint8_t>& data)
        {
            _serial_interface.send(
                typename dynamixel::instructions::Write<Protocol>(id, address, data));
            StatusPacket<Protocol> status;
            _serial_interface.recv(status);
        }

        // TODO: try to return a value, instead of using a reference
        template <typename T>
        void read(typename Protocol::id_t id, typename Protocol::address_t address,
            typename Protocol::length_t length, T& datum)
        {
            _serial_interface.send(
                typename dynamixel::instructions::Read<Protocol>(id, address, length));
            StatusPacket<Protocol> status;
            _serial_interface.recv(status);

            // unpack the data in the response and store it in data
            Protocol::unpack_data(status.parameters(), datum);
        }

        template <typename T>
        std::vector<std::pair<id_t, T>>
        read(typename Protocol::address_t address,
            typename Protocol::length_t length)
        {
            if (!_scanned)
                throw std::runtime_error("Reading a field for all connected "
                                         "servos requires that a scan be done "
                                         "beforehand.");

            std::vector<std::pair<id_t, T>> pairs;

            for (auto servo : _servos) {
                _serial_interface.send(
                    typename dynamixel::instructions::Read<Protocol>(
                        servo.second->id(),
                        address,
                        length));
                StatusPacket<Protocol> status;
                _serial_interface.recv(status);

                // unpack the data in the response and store it
                T datum;
                Protocol::unpack_data(status.parameters(), datum);
                pairs.push_back(std::make_pair(servo.second->id(), datum));
            }

            return pairs;
        }

        /** Change the ID of one or all actuators.
            If the target_id argument is set to the broadcast id for the current
            protocol, the IDs of all connected servos will be changed.

            @param id former ID of a servo
            @param new_id new ID of the servo responding to messages sent to target_id.
        **/
        void change_id(id_t id, id_t new_id)
        {
            StatusPacket<Protocol> status;
            if (Protocol::broadcast_id == id) {
                for (auto servo : _servos) {
                    _serial_interface.send(servo.second->set_id(new_id));
                    _serial_interface.recv(status);
                }
            }
            else {
                _serial_interface.send(_servos.at(id)->set_id(new_id));
                _serial_interface.recv(status);
            }
        }

        /** Change the baudrate of one or all actuators.
            If the target_id argument is set to the broadcast id for the current
            protocol, the IDs of all connected servos will be changed.

            @param target_id ID of a servo or the broadcast id
            @param baudrate new baudrete of the targetted servo(s).
        **/
        void change_baudrate(id_t id, unsigned baudrate)
        {
            baudrate = get_baudrate_id<Protocol>(baudrate);

            StatusPacket<Protocol> status;
            if (Protocol::broadcast_id == id) {
                for (auto servo : _servos) {
                    _serial_interface.send(servo.second->set_baudrate(baudrate));
                    _serial_interface.recv(status);
                }
            }
            else {
                _serial_interface.send(_servos.at(id)->set_baudrate(baudrate));
                _serial_interface.recv(status);
            }
        }

        /** Move one servo to a given angle

            @param id ID of the servo
            @param angle desired angle (rad)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
        **/
        void set_angle(id_t id, double angle)
        {
            StatusPacket<Protocol> status;
            _serial_interface.send(_servos.at(id)->set_goal_position_angle(angle));
            _serial_interface.recv(status);
        }

        /** Move servos to a given angle
            This version moves the selected servos all to the same angle

            @param ids vector of ids for the servos to be moved
            @param angle desired angle (rad)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
        **/
        void set_angle(const std::vector<id_t>& ids, double angle)
        {
            for (auto id : ids) {
                _serial_interface.send(_servos.at(id)->reg_goal_position_angle(angle));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(
                dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Move servos to a given angle
            This version moves each servo to its own angle

            @param ids vector of ids for the servos to be moved
            @param angles vector of angles, one for each actuator

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
            @throws runtime_error if the ids and angles vectors have different
                lengths
        **/
        // FIXME: use proper exception classes
        void set_angle(
            const std::vector<id_t>& ids,
            const std::vector<double>& angles)
        {
            if (ids.size() != angles.size())
                throw std::runtime_error("set_position(vector, vector): the "
                                         "vectors of IDs and angles should have "
                                         "the same length");

            for (int i = 0; i < ids.size(); i++) {
                _serial_interface.send(
                    _servos.at(ids[i])->reg_goal_position_angle(angles[i]));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            if (ids.size() > 0)
                _serial_interface.send(
                    dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Give current angular position (rad) of desired servos

            @param ids vector of ids for the servos to querry
            @return vector of angles in the same order as the ids

            @throws runtime_error if one actuator did not reply (within the timeout)
        **/
        std::vector<double> get_angles(const std::vector<id_t>& ids) const
        {
            std::vector<double> positions;

            for (auto id : ids) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(_servos.at(id)->get_present_position_angle());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid())
                    positions.push_back(
                        _servos.at(id)->parse_present_position_angle(status));
                else {
                    std::stringstream message;
                    message << "Did not receive any data when reading "
                            << id << "'s position";
                    throw std::runtime_error(message.str());
                }
            }

            return positions;
        }

        /** Give current angular position (rad) of all connected servos

            @return vector of all angles of the servos that were detected beforehand

            @throws runtime_error if one of the detected actuator did not reply
                (within the timeout)
        **/
        std::pair<std::vector<id_t>, std::vector<double>>
        get_angles() const
        {
            std::vector<double> positions;
            std::vector<id_t> ids;

            for (auto servo : _servos) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(
                    servo.second->get_present_position_angle());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid()) {
                    positions.push_back(
                        servo.second->parse_present_position_angle(status));
                    ids.push_back(servo.first);
                }
                else {
                    std::stringstream message;
                    message << "Did not receive any data when reading "
                            << servo.first << "'s position";
                    throw std::runtime_error(message.str());
                }
            }

            return std::make_pair(ids, positions);
        }

        /** Enable (or disable) an actuator.
            By default, it will enable the actuator, unless one gives the `enable`
            argument with value false. Here, enabling an acuator means that it
            will enable its position control or speed output.

            If the broadcast ID is used, it will scan through the servos and
            send them all the same instruction.

            Note: requires that detect_servos be run before

            @param id ID of a servo or Protocol::broadcast_id to select all
                connected servos
            @param enable whether the actuator should be enabled; if set to false
                it will be disabled
        **/
        void torque_enable(id_t id, bool enable = true)
        {
            StatusPacket<Protocol> status;
            if (Protocol::broadcast_id == id) {
                for (auto servo : _servos) {
                    _serial_interface.send(
                        servo.second->set_torque_enable((int)enable));
                    _serial_interface.recv(status);
                }
            }
            else {
                _serial_interface.send(
                    _servos.at(id)->set_torque_enable((int)enable));
                _serial_interface.recv(status);
            }
        }

    private:
        Usb2Dynamixel _serial_interface;
        std::map<typename Protocol::id_t, std::shared_ptr<BaseServo<Protocol>>>
            _servos;
        bool _scanned;
    };
}

#endif