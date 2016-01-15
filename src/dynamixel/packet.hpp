#ifndef DYNAMIXEL_PACKET_HPP_
#define DYNAMIXEL_PACKET_HPP_

#include <vector>
#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Packet {
    public:
        Packet(typename T::id_t id, typename T::instr_t instr, const std::vector<uint8_t>& parameters)
            : _packet(T::pack(id, instr, parameters)) {}

        const std::vector<uint8_t>& packet() const { return _packet; }

    protected:
        std::vector<uint8_t> _packet;
    };
}

#endif
