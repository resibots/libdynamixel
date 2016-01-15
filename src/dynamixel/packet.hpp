#ifndef DYNAMIXEL_PACKET_HPP_
#define DYNAMIXEL_PACKET_HPP_

#include <vector>
#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Packet {
    public:
        Packet(typename T::id_t id, std::vector<uint8_t> payload)
            : _packet(T::pack(id, payload)) {}

        const std::vector<uint8_t>& packet() const { return _packet; }

    protected:
        std::vector<uint8_t> _packet;
    };
}

#endif
