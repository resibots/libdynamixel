#ifndef DYNAMIXEL_OPERATING_MODE_HPP_
#define DYNAMIXEL_OPERATING_MODE_HPP_

#include "errors/error.hpp"
#include "servos.hpp"

#include <sstream>

namespace dynamixel {
    using namespace protocols;

    /**
        @throws errors since this function uses other parts of the library, it may
            relay an exception raised during its operation.
    **/
    template <class Controller>
    inline OperatingMode operating_mode_impl(
        Controller& controller,
        typename Protocol1::id_t id,
        typename Protocol1::address_t selected_protocol)
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
            message << "No viable response from the actuator " << (int)id << " from"
                    << " request for its counter-clockwise angle limit.";
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
    inline OperatingMode operating_mode_impl(
        Controller& controller,
        typename Protocol2::id_t id,
        typename Protocol2::address_t selected_protocol)
    {
        uint8_t mode;

        // read operating mode
        StatusPacket<Protocol2> status;
        controller.send(instructions::Read<Protocol2>(id, 11, 1));

        if (!controller.recv(status)) {
            std::stringstream message;
            message << "No viable response from the actuator " << (int)id << " from"
                    << " request for its clockwise angle limit.";
            throw errors::Error(message.str());
        }
        Protocol2::unpack_data(status.parameters(), mode);

        if (0 == mode)
            return OperatingMode::torque;
        if (1 == mode)
            return OperatingMode::wheel;
        else if (3 == mode)
            return OperatingMode::joint;
        else if (4 == mode)
            return OperatingMode::multi_turn;
        else
            return OperatingMode::unknown;
    }

    template <class Controller, class Protocol>
    OperatingMode operating_mode(Controller& controller, typename Protocol::id_t id)
    {
        // dummy variable to enforce protocol; only its type is used, its value is not used
        typename Protocol::address_t selected_protocol = 0;
        return operating_mode_impl(controller, id, selected_protocol);
    }
} // namespace dynamixel
#endif