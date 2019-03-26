#ifndef DYNAMIXEL_OPERATING_MODE_HPP_
#define DYNAMIXEL_OPERATING_MODE_HPP_

#include "errors/error.hpp"
#include "servos.hpp"

#include <sstream>
#include <string>

namespace dynamixel {
    using namespace protocols;

    /**
        @throws errors since this function uses other parts of the library, it may
            relay an exception raised during its operation.
    **/
    template <class Controller>
    inline OperatingMode operating_mode_p1(
        Controller& controller,
        typename Protocol1::id_t id)
    {
        uint16_t cw_angle_limit, ccw_angle_limit;

        // read clockwise angle limit
        StatusPacket<Protocol1> status;
        controller.send(instructions::Read<Protocol1>(id, 6, 2));

        if (!controller.recv(status)) {
            std::stringstream message;
            message << "No viable response from the actuator " << (int)id << " from"
                    << " request for its clockwise angle limit.";
            throw errors::Error(message.str());
        }
        Protocol1::unpack_data(status.parameters(), cw_angle_limit);

        // read counter-clockwise angle limit
        controller.send(instructions::Read<Protocol1>(id, 8, 2));

        if (!controller.recv(status)) {
            std::stringstream message;
            message << "No viable response from the actuator " << (int)id << " to the "
                    << "request for its counter-clockwise angle limit.";
            throw errors::Error(message.str());
        }
        Protocol1::unpack_data(status.parameters(), ccw_angle_limit);

        if (0 == cw_angle_limit && 0 == ccw_angle_limit)
            return OperatingMode::wheel;
        else if (4095 == cw_angle_limit && 4095 == ccw_angle_limit)
            return OperatingMode::multi_turn;
        else
            return OperatingMode::joint;
    }

    /**
        @throws errors since this function uses other parts of the library, it may
            relay an exception raised during its operation.
    **/
    template <class Controller>
    inline OperatingMode operating_mode_p2(
        Controller& controller,
        typename Protocol2::id_t id)
    {
        uint8_t mode;

        // read operating mode
        StatusPacket<Protocol2> status;
        controller.send(instructions::Read<Protocol2>(id, 11, 1));

        if (!controller.recv(status)) {
            std::stringstream message;
            message << "No viable response from the actuator " << (int)id << " to the "
                    << "request for its operating mode.";
            throw errors::Error(message.str());
        }
        Protocol2::unpack_data(status.parameters(), mode);

        if (0 == mode)
            return OperatingMode::torque;
        if (1 == mode)
            return OperatingMode::wheel;
        else if (2 == mode)
            return OperatingMode::joint;
        else if (3 == mode)
            return OperatingMode::joint;
        else if (4 == mode)
            return OperatingMode::multi_turn;
        else if (5 == mode)
            return OperatingMode::torque;
        else if (16 == mode)
            return OperatingMode::voltage;
        else
            return OperatingMode::unknown;
    }

    /** Query an actuator for its operating mode.

        Dynamixel actuators can be controlled in a few modes, depending on the model. For instance the Dynamixel Pro L54-50-S500-R has the following modes: position, torque, velocity and extended position control. For actuators using Protocol1, there are usually only two modes : position (joint) and velocity (wheel).

        For more informations on the modes, refer to the official Robotis documentation.
    **/
    template <class Protocol, class Controller>
    OperatingMode operating_mode(Controller& controller, typename Protocol::id_t id)
    {
        if (1 == Protocol::version)
            return operating_mode_p1(controller, id);
        else if (2 == Protocol::version)
            return operating_mode_p2(controller, id);
    }

    /// Give the string name for an operating mode
    std::string mode2str(OperatingMode mode)
    {
        switch (mode) {
        case OperatingMode::torque:
            return "torque";
            break;
        case OperatingMode::wheel:
            return "wheel";
            break;
        case OperatingMode::joint:
            return "joint";
            break;
        case OperatingMode::multi_turn:
            return "multi_turn";
            break;
        case OperatingMode::voltage:
            return "PWM (voltage)";
            break;
        case OperatingMode::unknown:
        default:
            return "unknown";
            break;
        }
    }
} // namespace dynamixel
#endif
