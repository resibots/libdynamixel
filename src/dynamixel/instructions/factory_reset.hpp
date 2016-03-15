#ifndef DYNAMIXEL_INSTRUCTIONS_FACTORY_RESET_HPP_
#define DYNAMIXEL_INSTRUCTIONS_FACTORY_RESET_HPP_

#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        template <class T>
        class FactoryReset : public InstructionPacket<T> {
        public:
            FactoryReset(typename T::id_t id) : InstructionPacket<T>(id, T::Instructions::factory_reset) {}
        };
    }
}

#endif
