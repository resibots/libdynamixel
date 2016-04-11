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
        const std::map<typename Protocol::address_t, std::shared_ptr<BaseServo<Protocol>>>&
        servos() const
        {
            if (!_scanned)
                throw std::runtime_error("You need to scan for connected "
                                         "actuators before trying to retrieve them");

            return _servos;
        }

        /** Change the ID of one or all actuators.
            If the target_id argument is set to the broadcast id for the current
            protocol, the IDs of all connected servos will be changed.

            @param id former ID of a servo
            @param new_id new ID of the servo responding to messages sent to target_id.
        **/
        void change_id(const long long int& id, const long long int& new_id)
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

        /** Move one servo to a given angle

            @param id ID of the servo
            @param angle desired angle (rad)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
        **/
        void set_angle(const long long int& id, double angle)
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
        void set_angle(const std::vector<long long int>& ids, double angle)
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
            const std::vector<long long int>& ids,
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
        std::vector<double> get_angles(const std::vector<long long int>& ids) const
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
        std::pair<std::vector<long long int>, std::vector<double>> get_angles() const
        {
            std::vector<double> positions;
            std::vector<long long int> ids;

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

    private:
        Usb2Dynamixel _serial_interface;
        std::map<typename Protocol::address_t, std::shared_ptr<BaseServo<Protocol>>>
            _servos;
        bool _scanned;
    };
}

#endif