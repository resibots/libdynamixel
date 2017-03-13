#ifndef DYNAMIXEL_INSTRUCTIONS_REBOOT_HPP_
#define DYNAMIXEL_INSTRUCTIONS_REBOOT_HPP_
// #include <typetraits>

#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        /** @ingroup instructions
        **/
        template <class T>
        class Reboot : public InstructionPacket<T> {
        public:
            Reboot(typename T::id_t id)
                : InstructionPacket<T>(id, T::Instructions::reboot) {}
        };
    }
}

#endif
