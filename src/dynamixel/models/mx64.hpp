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
            static const protocol_t::address_t id = 3;
            static const protocol_t::address_t baudrate = 4;
            typedef uint8_t baudrate_t;
            static const protocol_t::address_t torque_enable = 24;
            typedef uint8_t torque_enable_t;
            static const protocol_t::address_t led = 25;
            typedef uint8_t led_t;
             static const led_t led_off = 0;
            static const led_t led_on = 1;
            static const protocol_t::address_t goal_pos = 30;
            typedef uint16_t goal_pos_t;
            static const goal_pos_t min_goal_pos = 0;
            static const goal_pos_t max_goal_pos = 4095;
            static const protocol_t::address_t moving_speed = 32;
            typedef uint32_t moving_speed_t;
            static const protocol_t::address_t torque_limit = 34;
            typedef uint16_t torque_limit_t;
        };
    };

    class Mx64 : public ServoModel<Mx64> {    
    };
}
}

#endif
