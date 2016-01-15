#ifndef DYNAMIXEL_PING_HPP_
#define DYNAMIXEL_PING_HPP_

#include <stdint.h>

#include "instruction_packet.hpp"

namespace dynamixel {
    template <class T>
    class Ping : public InstructionPacket<T> {
    public:
        Ping(typename T::id_t id) : InstructionPacket<T>(id, T::Instructions::ping, std::vector<uint8_t>()) {}
    };
}

#endif
