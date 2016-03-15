#ifndef DYNAMIXEL_INSTRUCTIONS_PING_HPP_
#define DYNAMIXEL_INSTRUCTIONS_PING_HPP_

#include "../instruction_packet.hpp"

namespace dynamixel {
namespace instructions {
template <class T> class Ping : public InstructionPacket<T> {
public:
  Ping(typename T::id_t id) : InstructionPacket<T>(id, T::Instructions::ping) {}
};
}
}

#endif
