#ifndef DYNAMIXEL_READ_HPP_
#define DYNAMIXEL_READ_HPP_

#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Read : public Packet<T> {
    public:
        Read(typename T::id_t id, typename T::address_t address,
            typename T::length_t length)
            : Packet<T>(id, _get_payload(address, length)) {}

    protected:
        std::vector<uint8_t> _get_payload(uint8_t address, uint8_t length)
        {
            std::vector<uint8_t> payload(3);
            payload[0] = T::Instructions::read;
            payload[1] = address;
            payload[2] = length;
            return payload;
        }

        std::vector<uint8_t> _get_payload(uint16_t address, uint16_t length)
        {
            std::vector<uint8_t> payload(5);
            payload[0] = T::Instructions::read;
            payload[1] = (uint8_t)(address & 0x00ff);
            payload[2] = (uint8_t)((address >> 8) & 0x00ff);
            payload[3] = (uint8_t)(length & 0x00ff);
            payload[4] = (uint8_t)((length >> 8) & 0x00ff);
            return payload;
        }
    };
}

#endif
