#ifndef DYNAMIXEL_PING_HPP_
#define DYNAMIXEL_PING_HPP_

#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Ping : public Packet<T> {
    public:
        Ping(typename T::id_t id) : Packet<T>(id, _get_payload()) {}

    protected:
        std::vector<uint8_t> _get_payload()
        {
            std::vector<uint8_t> payload(1);
            payload[0] = T::Instructions::ping;
            return payload;
        }
    };
}

#endif
