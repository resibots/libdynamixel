#ifndef DYNAMIXEL_INSTRUCTION_PACKET_HPP_
#define DYNAMIXEL_INSTRUCTION_PACKET_HPP_

#include <vector>
#include <stdint.h>

///@defgroup instructions

namespace dynamixel {
    /**
        Abstract class for instructions

        @see Robotis' documentation for
            [protocol1](http://support.robotis.com/en/product/actuator/dynamixel/communication/dxl_packet.htm)
            and [protocol2](http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm)

    **/
    template <class Protocol>
    class InstructionPacket {
    public:
        InstructionPacket(typename Protocol::id_t id, typename Protocol::instr_t instr, const std::vector<uint8_t>& parameters)
            : _packet(Protocol::pack_instruction(id, instr, parameters)) {}

        InstructionPacket(typename Protocol::id_t id, typename Protocol::instr_t instr)
            : _packet(Protocol::pack_instruction(id, instr)) {}

        size_t size() const { return _packet.size(); }

        uint8_t operator[](size_t pos) const { return _packet[pos]; }

        const uint8_t* data() const { return &_packet.front(); }

    protected:
        std::vector<uint8_t> _packet;
    };
}

#endif
