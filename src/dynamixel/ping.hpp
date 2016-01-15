#ifndef DYNAMIXEL_PING_HPP_
#define DYNAMIXEL_PING_HPP_

#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Ping : public Packet<T> {
    public:
        Ping(typename T::id_t id) : Packet<T>(id, T::Instructions::ping, std::vector<uint8_t>()) {}
    };
}

#endif
