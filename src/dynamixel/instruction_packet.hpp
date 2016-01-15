#ifndef DYNAMIXEL_INSTRUCTION_PACKET_HPP_
#define DYNAMIXEL_INSTRUCTION_PACKET_HPP_

#include <vector>
#include <stdint.h>

namespace dynamixel {
    template <class T>
    class InstructionPacket {
    public:
        InstructionPacket(typename T::id_t id, typename T::instr_t instr, const std::vector<uint8_t>& parameters)
            : _packet(T::pack_instruction(id, instr, parameters)) {}

        size_t size() const { return _packet.size(); }

        uint8_t operator[](size_t pos) const { return _packet[pos]; } 

        const uint8_t* data() const { return &_packet.front(); }

    protected:
        std::vector<uint8_t> _packet;
    };
}

#endif
