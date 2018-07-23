#ifndef DYNAMIXEL_INSTRUCTIONS_READ_HPP_
#define DYNAMIXEL_INSTRUCTIONS_READ_HPP_

#include <stdint.h>

#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        /** @ingroup instructions
        **/
        template <class T>
        class Read : public InstructionPacket<T> {
        public:
            Read(typename T::id_t id, typename T::address_t address,
                typename T::length_t length)
                : InstructionPacket<T>(id, T::Instructions::read, _get_parameters(address, length)) {}

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
                parameters[0] = (uint8_t)(address & 0xFF);
                parameters[1] = (uint8_t)((address >> 8) & 0xFF);
                parameters[2] = (uint8_t)(length & 0xFF);
                parameters[3] = (uint8_t)((length >> 8) & 0xFF);
                return parameters;
            }
        };
    }
}

#endif
