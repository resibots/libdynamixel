#ifndef DYNAMIXEL_MX64_HPP_
#define DYNAMIXEL_MX64_HPP_

#include <stdint.h>

#include "servo.hpp"
#include "protocol1.hpp"

namespace dynamixel {
    class Mx64;

    template<>
    class ServoTraits<Mx64>
    {
        public:
        typedef Protocol1 protocol_t;

        struct CT {
            static const protocol_t::address_t led = 0x19;
            typedef uint8_t led_t;
            static const protocol_t::address_t goal_pos = 0x1E;
            typedef uint16_t goal_pos_t;
        };
    };

    class Mx64 : public Servo<Mx64> {    
    };
}

#endif
