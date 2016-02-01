#ifndef DYNAMIXEL_PRO_L42_10_S300_HPP_
#define DYNAMIXEL_PRO_L42_10_S300_HPP_

#include <stdint.h>

#include "servo.hpp"
#include "../protocol1.hpp"

namespace dynamixel {
namespace models {
    class ProL4210S300;

    template<>
    struct ModelTraits<ProL4210S300> {
        typedef Protocol2 protocol_t;

        struct CT {
            static const protocol_t::address_t model_number = 0;
            typedef uint16_t model_number_t;
            static const model_number_t model_number_value = 35072;
            static const protocol_t::address_t model_info = 2;
            typedef uint32_t model_info_t;
            static const protocol_t::address_t firmware_version = 6;
            typedef uint8_t firmware_version_t;
            static const protocol_t::address_t id = 7;
            typedef protocol_t::id_t id_t;
            static const protocol_t::address_t baudrate = 8;
            typedef uint8_t baudrate_t;
            static const protocol_t::address_t return_delay_time = 9;
            typedef uint8_t return_delay_time_t;
            static const protocol_t::address_t operating_mode = 11;
            typedef uint8_t operating_mode_t;
            static const protocol_t::address_t moving_threshold = 17;
            typedef uint32_t moving_threshold_t;
            static const protocol_t::address_t highest_temperature_limit = 21;
            typedef uint8_t highest_temperature_limit_t;
            static const protocol_t::address_t highest_voltage_limit = 22;
            typedef uint16_t highest_voltage_limit_t;
            static const protocol_t::address_t lowest_voltage_limit = 24;
            typedef uint16_t lowest_voltage_limit_t;
            static const protocol_t::address_t acceleration_limit = 26;
            typedef uint32_t acceleration_limit_t;
            static const protocol_t::address_t torque_limit = 30;
            typedef uint16_t torque_limit_t;
            static const protocol_t::address_t speed_limit = 32;
            typedef uint32_t speed_limit_t;
            static const protocol_t::address_t max_position_limit = 36;
            typedef int32_t max_position_limit_t;
            static const protocol_t::address_t min_position_limit = 40;
            typedef int32_t min_position_limit_t;
            static const protocol_t::address_t alarm_shutdown = 48;
            typedef uint8_t alarm_shutdown_t;
            static const protocol_t::address_t torque_enable = 562;
            typedef uint8_t torque_enable_t;
            static const protocol_t::address_t led_r = 563;
            typedef uint8_t led_r_t;
            static const protocol_t::address_t led_g = 564;
            typedef uint8_t led_g_t;
            static const protocol_t::address_t led_b = 565;
            typedef uint8_t led_b_t;
            static const protocol_t::address_t led = 563;
            typedef uint8_t led_t;
            static const uint8_t led_off = 0;
            static const uint8_t led_on = 255;
            static const protocol_t::address_t velocity_i_gain = 26;
            typedef uint16_t velocity_i_gain_t;
            static const protocol_t::address_t velocity_p_gain = 27;
            typedef uint16_t velocity_p_gain_t;
            static const protocol_t::address_t position_p_gain = 28;
            typedef uint16_t position_p_gain_t;
            static const protocol_t::address_t goal_position = 596;
            typedef int32_t goal_position_t;
            static const goal_position_t min_goal_position = -2047;
            static const goal_position_t max_goal_position = 2048;
            static const uint16_t min_goal_angle_deg = 0;
            static const uint16_t max_goal_angle_deg = 360;
            static const protocol_t::address_t moving_speed = 600;
            typedef uint32_t moving_speed_t;
            static const protocol_t::address_t goal_torque = 604;
            typedef uint16_t goal_torque_t;
            static const protocol_t::address_t goal_acceleration = 606;
            typedef uint32_t goal_acceleration_t;
            static const protocol_t::address_t moving = 610;
            typedef uint8_t moving_t;
            static const protocol_t::address_t present_position = 611;
            typedef int32_t present_position_t;
            static const protocol_t::address_t present_speed = 615;
            typedef uint32_t present_speed_t;
            static const protocol_t::address_t current = 621;
            typedef uint16_t current_t;
            static const protocol_t::address_t present_voltage = 623;
            typedef uint16_t present_voltage_t;
            static const protocol_t::address_t present_temperature = 625;
            typedef uint8_t present_temperature_t;
            static const protocol_t::address_t registered = 890;
            typedef uint8_t registered_t;
            static const protocol_t::address_t status_return_level = 891;
            typedef uint8_t status_return_level_t;
            static const protocol_t::address_t hardware_error_status = 892;
            typedef uint8_t hardware_error_status_t;
        };
    };

    class ProL4210S300 : public Servo<ProL4210S300> {
    public:
        typedef ProL4210S300 Model;

        ProL4210S300(typename protocol_t::id_t id) : Servo<ProL4210S300>(id) {};

        // Here we add the fields that are not common to all dynamixels
        READ_WRITE_FIELD(operating_mode);
        READ_WRITE_FIELD(moving_threshold);
        READ_WRITE_FIELD(acceleration_limit);
        READ_WRITE_FIELD(torque_limit);
        READ_WRITE_FIELD(speed_limit);
        READ_WRITE_FIELD(max_position_limit);
        READ_WRITE_FIELD(min_position_limit);
        READ_WRITE_FIELD(led_r);
        READ_WRITE_FIELD(led_g);
        READ_WRITE_FIELD(led_b);    
        READ_FIELD(led);
        WRITE_BOOL_FIELD(led, led); // To have an homogeneous way to light a led, we default the red one
        READ_WRITE_FIELD(velocity_i_gain);
        READ_WRITE_FIELD(velocity_p_gain);
        READ_WRITE_FIELD(position_p_gain);
        READ_WRITE_FIELD(goal_torque);
        READ_WRITE_FIELD(goal_acceleration);
        READ_FIELD(current);
        READ_FIELD(hardware_error_status);
    };
}
}

#endif
