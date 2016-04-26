#ifndef DYNAMIXEL_BAUDRATE_HPP_
#define DYNAMIXEL_BAUDRATE_HPP_

#include <stdint.h>
#include <memory>

#include "protocols.hpp"
#include "errors/error.hpp"

namespace dynamixel {
    /** The template specifications of this method are used to set the baudrate
        of the servos.

        Their internal memory represent the baudrates with special codes that we
        need to convert from the baudrate value.

        @param baudrate the baudrate we want the device to use
        @return hardware-specific code for the baudrate
    **/
    template <typename Protocol>
    inline uint8_t get_baudrate_id(unsigned int baudrate)
    {
        throw errors::Error("There is no baudrate table defined for this protocol");
    }

    template <>
    inline uint8_t get_baudrate_id<typename protocols::Protocol1>(unsigned int baudrate)
    {
        switch (baudrate) {
        case 9600:
            return 207;
        case 19200:
            return 103;
        case 57600:
            return 34;
        case 115200:
            return 16;
        case 200000:
            return 9;
        case 250000:
            return 7;
        case 400000:
            return 4;
        case 500000:
            return 3;
        case 1000000:
            return 1;
        default:
            std::stringstream err_message;
            err_message << "Invalid baudrate for protocol 1: " << baudrate;
            throw errors::Error(err_message.str());
        }
    }

    template <>
    inline uint8_t get_baudrate_id<typename protocols::Protocol2>(unsigned int baudrate)
    {
        switch (baudrate) {
        case 9600:
            return 0;
        case 57600:
            return 1;
        case 115200:
            return 2;
        case 1000000:
            return 3;
        case 2000000:
            return 4;
        case 3000000:
            return 5;
        case 4000000:
            return 6;
        case 4500000:
            return 7;
        case 10500000:
            return 8;
        default:
            std::stringstream err_message;
            err_message << "Invalid baudrate for protocol 2: " << baudrate;
            throw errors::Error(err_message.str());
        }
    }
}

#endif
