#ifndef DYNAMIXEL_INSTRUCTIONS_ACTION_HPP_
#define DYNAMIXEL_INSTRUCTIONS_ACTION_HPP_

#include "../instruction_packet.hpp"

namespace dynamixel {
namespace instructions {
    template <class T>
    class Action : public InstructionPacket<T> {
    public:
        Action(typename T::id_t id) : InstructionPacket<T>(id, T::Instructions::action) {}
    };
}
}

#endif
