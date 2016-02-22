#ifndef DYNAMIXEL_SERVOS_XL320_HPP_
#define DYNAMIXEL_SERVOS_XL320_HPP_

#include <stdint.h>

#include "servo.hpp"
#include "../protocols/protocol2.hpp"

namespace dynamixel {
namespace servos {
    class Xl320;

    template<>
    struct ModelTraits<Xl320> {
        typedef protocols::Protocol2 protocol_t;

        struct CT {
            static const protocol_t::address_t model_number = 0;
            typedef uint16_t model_number_t;
            static const model_number_t model_number_value = 350;
            static const protocol_t::address_t firmware_version = 2;
            typedef uint8_t firmware_version_t;
            static const protocol_t::address_t id = 3;
            typedef protocol_t::id_t id_t;
            static const protocol_t::address_t baudrate = 4;
            typedef uint8_t baudrate_t;
            static const protocol_t::address_t return_delay_time = 5;
            typedef uint8_t return_delay_time_t;
            static const protocol_t::address_t cw_angle_limit = 6;
            typedef uint16_t cw_angle_limit_t;
            static const protocol_t::address_t ccw_angle_limit = 8;
            typedef uint16_t ccw_angle_limit_t;
            static const protocol_t::address_t control_mode = 11;
            typedef uint8_t control_mode_t;
            static const protocol_t::address_t highest_temperature_limit = 12;
            typedef uint8_t highest_temperature_limit_t;
            static const protocol_t::address_t lowest_voltage_limit = 13;
            typedef uint8_t lowest_voltage_limit_t;
            static const protocol_t::address_t highest_voltage_limit = 14;
            typedef uint8_t highest_voltage_limit_t;
            static const protocol_t::address_t max_torque = 15;
            typedef uint16_t max_torque_t;
            static const protocol_t::address_t status_return_level = 17;
            typedef uint8_t status_return_level_t;
            static const protocol_t::address_t alarm_shutdown = 18;
            typedef uint8_t alarm_shutdown_t;
            static const protocol_t::address_t torque_enable = 24;
            typedef uint8_t torque_enable_t;
            static const protocol_t::address_t led = 25;
            typedef uint8_t led_t;
            static const protocol_t::address_t d_gain = 27;
            typedef uint8_t d_gain_t;
            static const protocol_t::address_t i_gain = 28;
            typedef uint8_t i_gain_t;
            static const protocol_t::address_t p_gain = 29;
            typedef uint8_t p_gain_t;
            static const protocol_t::address_t goal_position = 30;
            typedef uint16_t goal_position_t;
            static const goal_position_t min_goal_position = 0;
            static const goal_position_t max_goal_position = 1023;
            static const uint16_t min_goal_angle_deg = 30;
            static const uint16_t max_goal_angle_deg = 330;
            static const protocol_t::address_t moving_speed = 32;
            typedef uint16_t moving_speed_t;
            static const protocol_t::address_t goal_torque = 35;
            typedef uint16_t goal_torque_t;
            static const protocol_t::address_t present_position = 37;
            typedef uint16_t present_position_t;
            static const protocol_t::address_t present_speed = 39;
            typedef uint16_t present_speed_t;
            static const protocol_t::address_t present_load = 41;
            typedef uint16_t present_load_t;
            static const protocol_t::address_t present_voltage = 45;
            typedef uint8_t present_voltage_t;
            static const protocol_t::address_t present_temperature = 46;
            typedef uint8_t present_temperature_t;
            static const protocol_t::address_t registered = 47;
            typedef uint8_t registered_t;
            static const protocol_t::address_t moving = 49;
            typedef uint8_t moving_t;
            static const protocol_t::address_t hardware_error_status = 50;
            typedef uint8_t hardware_error_status_t;
            static const protocol_t::address_t punch = 51;
            typedef uint16_t punch_t;
        };
    };

    class Xl320 : public Servo<Xl320> {
    public:
        typedef Xl320 Model;

        Xl320(typename protocol_t::id_t id) : Servo<Xl320>(id) {};

        MODEL_NAME(Xl320);

        // Here we add the fields that are not common to all dynamixels
        READ_WRITE_FIELD(cw_angle_limit);
        READ_WRITE_FIELD(ccw_angle_limit);
        READ_WRITE_FIELD(control_mode);
        READ_WRITE_FIELD(max_torque);
        READ_WRITE_FIELD(led);
        READ_WRITE_FIELD(d_gain);
        READ_WRITE_FIELD(i_gain);
        READ_WRITE_FIELD(p_gain);
        READ_WRITE_FIELD(goal_torque);
        READ_FIELD(present_load);
        READ_FIELD(hardware_error_status);
        READ_WRITE_FIELD(punch);
    };
}
}

#endif
