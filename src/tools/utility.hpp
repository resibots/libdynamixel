#ifndef DYNAMIXEL_UTILITY
#define DYNAMIXEL_UTILITY

#include <dynamixel/dynamixel.hpp>

#include <map>
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
        Utility(const std::string& name, int baudrate = B115200, double recv_timeout = 0.1) : _serial_interface(name, baudrate, recv_timeout), _scanned(false)
        {
        }

        void detect_servos(double scan_timeout = 0.01)
        {
            double original_timeout = _serial_interface.recv_timeout();
            _serial_interface.set_recv_timeout(scan_timeout);

            _servos = auto_detect<Protocol>(_serial_interface);
            _scanned = true;

            _serial_interface.set_recv_timeout(original_timeout);

            // Build a map from the actuator id to the index in _servos
            typename std::vector<std::shared_ptr<BaseServo<Protocol>>>::iterator servo_it;
            for (servo_it = _servos.begin(); servo_it != _servos.end(); ++servo_it) {
                _ids_map[(*servo_it)->id()] = servo_it - _servos.begin();
            }
        }

        /** Return the list of connected actuators.
            If we didn't do a scanning yet, does it with the default recieve timout.

            @return vector of (std) shared pointers to BaseServo instances
        **/
        const std::vector<std::shared_ptr<BaseServo<Protocol>>>& list()
        {
            if (!_scanned)
                detect_servos();

            return _servos;
        }

        const std::map<long long int, unsigned>& ids_map() const
        {
            if (!_scanned)
                throw std::runtime_error("ids_map(): you need to scan for connected actuators before asking for the map of IDs");

            return _ids_map;
        }

        const std::map<long long int, unsigned>& ids_map()
        {
            return _ids_map;
        }

        void set_angle(long long int id, double angle)
        {
            // FIXME: check that the id indeed matches one of the detected servos
            // FIXME: fix the exception thrown if we ask an angle beyond limits. Currently, the message speaks about ticks whereas we are talking in rad.
            _serial_interface.send(_servos[id]->set_goal_position_angle(angle));
        }

        void set_angle(std::vector<long long int> ids, double angle)
        {
            for (auto id : ids) {
                std::shared_ptr<BaseServo<Protocol>> servo = _servos[_ids_map[id]];
                _serial_interface.send(servo->reg_goal_position_angle(angle));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        void set_angle(std::vector<long long int> ids, std::vector<double> angles)
        {
            if (ids.size() != angles.size())
                throw std::runtime_error("set_position(vector, vector): the "
                                         "vectors of IDs and angles should have "
                                         "the same length");

            for (int i = 0; i < ids.size(); i++) {
                std::shared_ptr<BaseServo<Protocol>> servo = _servos[_ids_map[ids[i]]];
                _serial_interface.send(servo->reg_goal_position_angle(angles[i]));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        std::vector<double> get_angles(std::vector<long long int> ids)
        {
            std::vector<double> positions(ids.size());

            for (unsigned i = 0; i < ids.size(); ++i) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(
                    _servos[_ids_map[ids[i]]]->get_present_position_angle());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid())
                    positions[i] = _servos[_ids_map[ids[i]]]
                                       ->parse_present_position_angle(status);
                else {
                    std::stringstream message;
                    message << "Did not receive any data when reading "
                            << ids[i] << "'s position";
                    throw std::runtime_error(message.str());
                }
            }

            return positions;
        }

        std::vector<double> get_angles()
        {
            std::vector<double> positions(_servos.size());

            for (unsigned i = 0; i < _servos.size(); ++i) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(
                    _servos[i]->get_present_position_angle());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid())
                    positions[i] = _servos[i]
                                       ->parse_present_position_angle(status);
                else {
                    std::stringstream message;
                    message << "Did not receive any data when reading "
                            << _servos[i]->id() << "'s position";
                    throw std::runtime_error(message.str());
                }
            }

            return positions;
        }

    private:
        Usb2Dynamixel _serial_interface;
        std::vector<std::shared_ptr<BaseServo<Protocol>>> _servos;
        std::map<long long int, unsigned> _ids_map;
        bool _scanned;
    };
}

#endif