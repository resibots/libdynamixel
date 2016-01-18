#ifndef DYNAMIXEL_MX64_HPP_
#define DYNAMIXEL_MX64_HPP_

#include <stdint.h>

#include "servo_model.hpp"
#include "../protocol1.hpp"

namespace dynamixel {
namespace models {
    class Mx64;

    template<>
    struct ModelTraits<Mx64> {
        typedef Protocol1 protocol_t;

        struct CT {
            static const protocol_t::address_t led = 0x19;
            typedef uint8_t led_t;
             static const led_t led_off = 0x00;
            static const led_t led_on = 0x01;
            static const protocol_t::address_t goal_pos = 0x1E;
            typedef uint16_t goal_pos_t;
        };
    };

    class Mx64 : public ServoModel<Mx64> {    
    };
}
}

#endif
