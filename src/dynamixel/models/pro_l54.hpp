#ifndef DYNAMIXEL_PRO_L54_HPP_
#define DYNAMIXEL_PRO_L54_HPP_

#include <stdint.h>

#include "servo_model.hpp"
#include "../protocol1.hpp"

namespace dynamixel {
namespace models {
    class ProL54;

    template<>
    struct ModelTraits<ProL54> {
        typedef Protocol2 protocol_t;

        struct CT {
            static const protocol_t::address_t led = 0x233;
            typedef uint8_t led_t;
            static const led_t led_off = 0x00;
            static const led_t led_on = 0xFF;
            static const protocol_t::address_t goal_pos = 0x254;
            typedef uint32_t goal_pos_t;
        };
    };

    class ProL54 : public ServoModel<ProL54> {    
    };
}
}

#endif
