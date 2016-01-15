#ifndef DYNAMIXEL_READ_HPP_
#define DYNAMIXEL_READ_HPP_

#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Read : public Packet<T> {
    public:
        Read(typename T::id_t id, typename T::address_t address,
            typename T::length_t length)
            : Packet<T>(id, T::Instructions::read, _get_parameters(address, length)) {}

    protected:
        std::vector<uint8_t> _get_parameters(uint8_t address, uint8_t length)
        {
            std::vector<uint8_t> parameters(2);
            parameters[0] = address;
            parameters[1] = length;
            return parameters;
        }

        std::vector<uint8_t> _get_parameters(uint16_t address, uint16_t length)
        {
            std::vector<uint8_t> parameters(4);
            parameters[0] = (uint8_t)(address & 0x00ff);
            parameters[1] = (uint8_t)((address >> 8) & 0x00ff);
            parameters[2] = (uint8_t)(length & 0x00ff);
            parameters[3] = (uint8_t)((length >> 8) & 0x00ff);
            return parameters;
        }
    };
}

#endif
