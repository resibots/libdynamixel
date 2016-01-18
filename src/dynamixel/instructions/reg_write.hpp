#ifndef DYNAMIXEL_INSTRUCTIONS_REG_WRITE_HPP_
#define DYNAMIXEL_INSTRUCTIONS_REG_WRITE_HPP_

#include <stdint.h>

#include "../instruction_packet.hpp"

namespace dynamixel {
namespace instructions {
    template <class T>
    class RegWrite : public InstructionPacket<T> {
    public:
        RegWrite(typename T::id_t id, typename T::address_t address,
            const std::vector<uint8_t>& data)
            : InstructionPacket<T>(id, T::Instructions::reg_write, _get_parameters(address, data)) {}

    protected:
        std::vector<uint8_t> _get_parameters(uint8_t address,
            const std::vector<uint8_t>& data)
        {
            std::vector<uint8_t> parameters(1 + data.size());
            parameters[0] = address;

            for (size_t i = 0; i < data.size(); ++i)
                parameters[1 + i] = data[i];

            return parameters;
        }

        std::vector<uint8_t> _get_parameters(uint16_t address,
            const std::vector<uint8_t>& data)
        {
            std::vector<uint8_t> parameters(2 + data.size());
            parameters[0] = (uint8_t)(address & 0xFF);
            parameters[1] = (uint8_t)((address >> 8) & 0xFF);

            for (size_t i = 0; i < data.size(); ++i)
                parameters[2 + i] = data[i];

            return parameters;
        }
    };
}
}

#endif
