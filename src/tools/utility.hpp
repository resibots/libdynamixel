#ifndef DYNAMIXEL_UTILITY
#define DYNAMIXEL_UTILITY

#include <dynamixel/dynamixel.hpp>
#include <tools/utility_error.hpp>

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

        /**
            @throws dynamixel::errors:Error if an issue was met while attempting
            to open the serial interface
        **/
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

            @throws dynamixel::error::UnpackError from auto_detect_map
            @throws dynamixel::error:Error from auto_detect_map

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

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        const std::map<typename Protocol::id_t, std::shared_ptr<BaseServo<Protocol>>>&
        servos() const
        {
            check_scanned();

            return _servos;
        }

        /** Write a data field in a servo's memory.

            @param id targetted servo
            @param address address of the first memory block to be modified
            @param data new value to be written
        **/
        template <typename T>
        void write(typename Protocol::id_t id, typename Protocol::address_t address,
            T data)
        {
            _serial_interface.send(
                typename dynamixel::instructions::Write<Protocol>(
                    id,
                    address,
                    Protocol::pack_data(data)));
            StatusPacket<Protocol> status;
            _serial_interface.recv(status);
        }

        /** Write a data field in all connected servo's memories.

            All connected servos will be affected. Requires also to call
            detect_servos beforehand.

            @param address address of the first memory block to be modified
            @param data new value to be written

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        template <typename T>
        void write(typename Protocol::address_t address, T data)
        {
            check_scanned();

            for (auto servo : _servos) {
                _serial_interface.send(
                    typename dynamixel::instructions::RegWrite<Protocol>(
                        servo.second->id(),
                        address,
                        Protocol::pack_data(data)));
                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(
                dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Read a given number of bytes in a servo's memory and return them.

            The method is templated by it's return type. Here are the admitted
            template parameters:
            - uint8_t
            - uint16_t
            - uint32_t
            - int32_t

            @param id ID of the requested servo
            @param address address to the first byte to read in the servo's memory
            @return data retrieved from the servo
        **/
        template <typename T>
        T read(typename Protocol::id_t id, typename Protocol::address_t address)
        {
            _serial_interface.send(
                typename dynamixel::instructions::Read<Protocol>(id, address,
                    sizeof(T)));
            StatusPacket<Protocol> status;
            if (!_serial_interface.recv(status)) {
                std::stringstream message;
                message << "No packet received when timeout ("
                        << _serial_interface.recv_timeout()
                        << " s) was reached";
                throw errors::Error(message.str());
            }

            // Unpack the data in the response and store it
            T datum;
            Protocol::unpack_data(status.parameters(), datum);
            return datum;
        }

        /** Read a given number of bytes in all servo's memories and return them.
            All connected servos are asked to return the data in it's memory.
            Requires a prior detection of connected servos (@see detect_servos).

            The method is templated by it's return type. Here are the admitted
            template parameters:
            - uint8_t
            - uint16_t
            - uint32_t
            - int32_t

            @param address address to the first byte to read in the servo's memory
            @param length number of bytes to read
            @return vector of pairs; each pair has an ID and a datum

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        template <typename T>
        std::vector<std::pair<id_t, T>>
        read(typename Protocol::address_t address)
        {
            check_scanned();

            std::vector<std::pair<id_t, T>> pairs;

            for (auto servo : _servos) {
                _serial_interface.send(
                    typename dynamixel::instructions::Read<Protocol>(
                        servo.second->id(),
                        address,
                        sizeof(T)));
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

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void change_id(id_t id, id_t new_id)
        {
            check_scanned();

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
            protocol, the baudrate of all connected servos will be changed.

            @param target_id ID of a servo or the broadcast id
            @param baudrate new baudrete of the targetted servo(s).

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void change_baudrate(id_t id, unsigned baudrate)
        {
            check_scanned();

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

        /** Reset the Control Table to its initial factory default settings.
            If the target_id argument is set to the broadcast id for the current
            protocol, all connected servos will be affected.

            @param target_id ID of a servo or the broadcast id

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void factory_reset(id_t id)
        {
            check_scanned();

            StatusPacket<Protocol> status;
            if (Protocol::broadcast_id == id) {
                for (auto servo : _servos) {
                    _serial_interface.send(FactoryReset<Protocol>(servo.first));
                    _serial_interface.recv(status);
                }
            }
            else {
                _serial_interface.send(FactoryReset<Protocol>(id));
                _serial_interface.recv(status);
            }
        }

        /** Move one servo to a given angle

            @param id ID of the servo
            @param angle desired angle (rad)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void set_angle(id_t id, double angle)
        {
            check_scanned();

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
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void set_angle(const std::vector<id_t>& ids, double angle)
        {
            check_scanned();

            for (auto id : ids) {
                _serial_interface.send(_servos.at(id)->reg_goal_position_angle(angle));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(
                dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Move servos to a given angle
            This version moves all servos to the same angle

            @param angle desired angle (rad)

            @throws dynamixel::errors::ServoLimitError if angle is out of the
                servo's feasible positions
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void set_angle(double angle)
        {
            check_scanned();

            for (auto servo : _servos) {
                _serial_interface.send(servo.second->reg_goal_position_angle(angle));

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
            @throws errors::UtilityError if the ids and angles vectors have different
                lengths or if you didn't detect connected servos before
        **/
        void set_angle(
            const std::vector<id_t>& ids,
            const std::vector<double>& angles)
        {
            check_scanned();
            if (ids.size() != angles.size())
                throw errors::UtilityError("set_position(vector, vector): the "
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

            @throws errors::Error if one actuator did not reply (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::vector<double> get_angle(const std::vector<id_t>& ids) const
        {
            check_scanned();

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
                    message << (int)id << " did not answer to the request for "
                            << "its position";
                    throw errors::Error(message.str());
                }
            }

            return positions;
        }

        /** Give current angular position (rad) of all connected servos

            @return vector of all angles of the servos that were detected beforehand

            @throws errors::Error if one of the detected actuator did not reply
                (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::pair<std::vector<id_t>, std::vector<double>>
        get_angle() const
        {
            check_scanned();

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
                    message << (int)servo.first << " did not answer to the request for "
                            << "its position";
                    throw errors::Error(message.str());
                }
            }

            return std::make_pair(ids, positions);
        }

        /** Give a speed target for a given set of servos.
            This version gives the selected servos one common speed target.

            Note: the behavior of the actuator depends on its control mode
                (wheel/continuous or position).

            @param ids vector of ids for the concerned servos
            @param speed desired speed (rad/s)
            @param wheel_mode boolean set to true if the actuators are in wheel
                mode (defaults to false)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if speed is out of the
                servo's feasible bounds
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void set_speed(const std::vector<id_t>& ids, double speed,
            bool wheel_mode = false)
        {
            check_scanned();

            // the new speed is sent to each actuator but they wait for the
            // "Action" (see bellow) command to enact the change
            for (auto id : ids) {
                _serial_interface.send(_servos.at(id)->reg_moving_speed_angle(
                    speed,
                    wheel_mode ? cst::wheel : cst::joint));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(
                dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Give a speed target for a given set of servos.
            This version gives all servos the same speed target.

            Note: the behavior of the actuator depends on its control mode
                (wheel/continuous or position).

            @param speed desired speed (rad/s)
            @param wheel_mode boolean set to true if the actuators are in wheel
                mode (defaults to false)

            @throws dynamixel::errors::ServoLimitError if speed is out of the
                servo's feasible bounds
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void set_speed(double speed, bool wheel_mode = false)
        {
            check_scanned();

            // the new speed is sent to each actuator but they wait for the
            // "Action" (see bellow) command to enact the change
            for (auto servo : _servos) {
                _serial_interface.send(servo.second->reg_moving_speed_angle(speed,
                    wheel_mode ? cst::wheel : cst::joint));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            _serial_interface.send(
                dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Give a speed target for a given set of servos.
            This version gives each servo a specific speed target.

            Note: the behavior of the actuator depends on its control mode
                (wheel/continuous or position).

            @param ids vector of ids for the servos to be moved
            @param speed vector of angular velocities (rad/s), one for each actuator
            @param wheel_mode boolean set to true if the actuators are in wheel
                mode (defaults to false)

            @throws out_of_range if the id is not among the detected servos
            @throws dynamixel::errors::ServoLimitError if speed is out of the
                servo's feasible bounds
            @throws errors::UtilityError if the ids and speed vectors have different
                lengths or if you didn't detect connected servos before
        **/
        void set_speed(
            const std::vector<id_t>& ids,
            const std::vector<double>& speeds, bool wheel_mode = false)
        {
            check_scanned();
            if (ids.size() != speeds.size())
                throw errors::UtilityError("set_speed(vector, vector): the "
                                           "vectors of IDs and speeds should "
                                           "have the same length");

            for (int i = 0; i < ids.size(); i++) {
                _serial_interface.send(
                    _servos.at(ids[i])->reg_moving_speed_angle(speeds[i],
                        wheel_mode ? cst::wheel : cst::joint));

                StatusPacket<Protocol> status;
                _serial_interface.recv(status);
            }

            if (ids.size() > 0)
                _serial_interface.send(
                    dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));
        }

        /** Give goal angular velocity (rad/s) of desired servos

            @param ids vector of ids for the servos to querry
            @return vector of anglular velocities in the same order as the ids

            @throws errors::Error if one actuator did not reply (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::vector<double> get_speed(const std::vector<id_t>& ids) const
        {
            check_scanned();

            std::vector<double> speeds;

            for (auto id : ids) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(_servos.at(id)->get_moving_speed());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid())
                    speeds.push_back(
                        _servos.at(id)->parse_joint_speed(status));
                else {
                    std::stringstream message;
                    message << (int)id << " did not answer to the request for "
                            << "its position";
                    throw errors::Error(message.str());
                }
            }

            return speeds;
        }

        /** Give goal angular velocity (rad/s) of all connected servos

            @return vector of all anglular velocities of the servos that were
                detected beforehand

            @throws errors::Error if one of the detected actuator did not reply
                (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::pair<std::vector<id_t>, std::vector<double>>
        get_speed() const
        {
            check_scanned();

            std::vector<double> speeds;
            std::vector<id_t> ids;

            for (auto servo : _servos) {
                StatusPacket<Protocol> status;
                // request current position
                _serial_interface.send(
                    servo.second->get_moving_speed());
                _serial_interface.recv(status);

                // parse response to get the position
                if (status.valid()) {
                    speeds.push_back(
                        servo.second->parse_joint_speed(status));
                    ids.push_back(servo.first);
                }
                else {
                    std::stringstream message;
                    message << (int)servo.first << " did not answer to the request for "
                            << "its position";
                    throw errors::Error(message.str());
                }
            }

            return std::make_pair(ids, speeds);
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

            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        void torque_enable(id_t id, bool enable = true)
        {
            check_scanned();

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

        /** Tell whether the selected actuators' torque is enabled.

            @return vector of enablement status for each actuator (boolean)

            @throws errors::Error if one of the detected actuator did not reply
                (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::vector<bool>
        get_torque_enable(std::vector<id_t> ids)
        {
            check_scanned();

            std::vector<bool> torque_enable;

            for (auto id : ids) {
                StatusPacket<Protocol> status;

                // request whether torque is enabled
                _serial_interface.send(_servos.at(id)->get_torque_enable());
                _serial_interface.recv(status);

                // parse response to know whether torque is enabled
                if (status.valid())
                    torque_enable.push_back(
                        _servos.at(id)->parse_torque_enable(status));
                else {
                    std::stringstream message;
                    message << (int)id << " did not answer to the request for "
                            << "its torque enabling status";
                    throw errors::Error(message.str());
                }
            }

            return torque_enable;
        }

        /** Tell whether the selected actuators' torque is enabled.

            @return std::pair which first element is the vector with the IDs of
                each actuator and the second element is a vector of enablement
                status associated to the actuator (boolean)

            @throws errors::Error if one of the detected actuator did not reply
                (within the timeout)
            @throws errors::UtilityError if you didn't detect connected servos before
        **/
        std::pair<std::vector<id_t>, std::vector<bool>>
        get_torque_enable()
        {
            check_scanned();

            std::vector<id_t> ids;
            std::vector<bool> torque_enable;

            for (auto servo : _servos) {
                StatusPacket<Protocol> status;

                // request whether torque is enabled
                _serial_interface.send(
                    servo.second->get_torque_enable());
                _serial_interface.recv(status);

                // parse response to know whether torque is enabled
                if (status.valid()) {
                    torque_enable.push_back(
                        servo.second->parse_torque_enable(status));
                    ids.push_back(servo.first);
                }
                else {
                    std::stringstream message;
                    message << (int)servo.first << " did not answer to the request for "
                            << "its torque enabling status";
                    throw errors::Error(message.str());
                }
            }

            return std::make_pair(ids, torque_enable);
        }

    protected:
        /** Check that we scanned for connected servos and throws an exception
            otherwise.

            @throws dynamixel::errors::UtilityError if scanning was not done
        **/
        inline void check_scanned() const
        {
            if (!_scanned)
                throw errors::UtilityError("You need to scan for connected "
                                           "actuators before trying to retrieve them");
        }

    private:
        Usb2Dynamixel _serial_interface;
        std::map<typename Protocol::id_t, std::shared_ptr<BaseServo<Protocol>>>
            _servos;
        bool _scanned;
    };
}

#endif