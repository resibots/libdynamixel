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
            static const protocol_t::address_t id = 7;
            static const protocol_t::address_t baudrate = 8;
            typedef uint8_t baudrate_t;
            static const protocol_t::address_t torque_enable = 562;
            typedef uint8_t torque_enable_t;
            static const protocol_t::address_t led = 563;
            typedef uint8_t led_t;
            static const led_t led_off = 0;
            static const led_t led_on = 255;
            static const protocol_t::address_t goal_pos = 596;
            typedef int32_t goal_pos_t;
            static const goal_pos_t min_goal_pos = -180684;
            static const goal_pos_t max_goal_pos = 180684;
            static const protocol_t::address_t moving_speed = 600;
            typedef uint32_t moving_speed_t;
            static const protocol_t::address_t torque_limit = 604;
            typedef uint16_t torque_limit_t;
        };
    };

    class ProL54 : public ServoModel<ProL54> {    
    };
}
}

#endif
