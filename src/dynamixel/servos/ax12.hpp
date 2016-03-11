#ifndef DYNAMIXEL_SERVOS_AX12_HPP_
#define DYNAMIXEL_SERVOS_AX12_HPP_

#include <stdint.h>

#include "servo.hpp"
#include "../protocols/protocol1.hpp"

namespace dynamixel {
    namespace servos {
        class Ax12;

        template <>
        struct ModelTraits<Ax12> {
            typedef protocols::Protocol1 protocol_t;

            struct CT {
                static const protocol_t::address_t model_number = 0;
                typedef uint16_t model_number_t;
                static const model_number_t model_number_value = 12;
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
                static const protocol_t::address_t highest_temperature_limit = 11;
                typedef uint8_t highest_temperature_limit_t;
                static const protocol_t::address_t lowest_voltage_limit = 12;
                typedef uint8_t lowest_voltage_limit_t;
                static const protocol_t::address_t highest_voltage_limit = 13;
                typedef uint8_t highest_voltage_limit_t;
                static const protocol_t::address_t max_torque = 14;
                typedef uint16_t max_torque_t;
                static const protocol_t::address_t status_return_level = 16;
                typedef uint8_t status_return_level_t;
                static const protocol_t::address_t alarm_led = 17;
                typedef uint8_t alarm_led_t;
                static const protocol_t::address_t alarm_shutdown = 18;
                typedef uint8_t alarm_shutdown_t;
                static const protocol_t::address_t torque_enable = 24;
                typedef uint8_t torque_enable_t;
                static const protocol_t::address_t led = 25;
                typedef uint8_t led_t;
                static const protocol_t::address_t cw_compliance_margin = 26;
                typedef uint8_t cw_compliance_margin_t;
                static const protocol_t::address_t ccw_compliance_margin = 27;
                typedef uint8_t ccw_compliance_margin_t;
                static const protocol_t::address_t cw_compliance_slope = 28;
                typedef uint8_t cw_compliance_slope_t;
                static const protocol_t::address_t ccw_compliance_slope = 29;
                typedef uint8_t ccw_compliance_slope_t;
                static const protocol_t::address_t goal_position = 30;
                typedef uint16_t goal_position_t;
                static const goal_position_t min_goal_position = 0;
                static const goal_position_t max_goal_position = 1023;
                static const uint16_t min_goal_angle_deg = 30;
                static const uint16_t max_goal_angle_deg = 330;
                static const protocol_t::address_t moving_speed = 32;
                typedef uint16_t moving_speed_t;
                static const protocol_t::address_t torque_limit = 34;
                typedef uint16_t torque_limit_t;
                static const protocol_t::address_t present_position = 36;
                typedef uint16_t present_position_t;
                static const protocol_t::address_t present_speed = 38;
                typedef uint16_t present_speed_t;
                static const protocol_t::address_t present_load = 40;
                typedef uint16_t present_load_t;
                static const protocol_t::address_t present_voltage = 42;
                typedef uint8_t present_voltage_t;
                static const protocol_t::address_t present_temperature = 43;
                typedef uint8_t present_temperature_t;
                static const protocol_t::address_t registered = 44;
                typedef uint8_t registered_t;
                static const protocol_t::address_t moving = 46;
                typedef uint8_t moving_t;
                static const protocol_t::address_t lock = 47;
                typedef uint8_t lock_t;
                static const protocol_t::address_t punch = 48;
                typedef uint16_t punch_t;
            };
        };

        class Ax12 : public Servo<Ax12> {
        public:
            typedef Ax12 Model;

            Ax12(typename protocol_t::id_t id) : Servo<Ax12>(id){};

            MODEL_NAME(Ax12);

            // Here we add the fields that are not common to all dynamixels
            READ_WRITE_FIELD(cw_angle_limit);
            READ_WRITE_FIELD(ccw_angle_limit);
            READ_WRITE_FIELD(max_torque);
            READ_WRITE_FIELD(alarm_led);
            READ_WRITE_FIELD(led);
            READ_WRITE_FIELD(cw_compliance_margin);
            READ_WRITE_FIELD(ccw_compliance_margin);
            READ_WRITE_FIELD(cw_compliance_slope);
            READ_WRITE_FIELD(ccw_compliance_slope);
            READ_WRITE_FIELD(torque_limit);
            READ_FIELD(present_load);
            READ_WRITE_FIELD(lock);
            READ_WRITE_FIELD(punch);
        };
    }
}

#endif
