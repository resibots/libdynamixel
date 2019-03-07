#ifndef DYNAMIXEL_SERVOS_XL430W250_HPP_
#define DYNAMIXEL_SERVOS_XL430W250_HPP_

#include <stdint.h>

#include "../protocols/protocol2.hpp"
#include "servo.hpp"

namespace dynamixel {
    namespace servos {
        class Xl430W250;

        template <>
        struct ModelTraits<Xl430W250> {
            typedef protocols::Protocol2 protocol_t;

            struct CT {
                static const protocol_t::address_t model_number = 0;
                typedef uint16_t model_number_t;
                static const model_number_t model_number_value = 1060;
                static const protocol_t::address_t model_information = 2;
                typedef uint32_t model_information_t;
                static const protocol_t::address_t firmware_version = 6;
                typedef uint8_t firmware_version_t;
                static const protocol_t::address_t id = 7;
                typedef protocol_t::id_t id_t;
                static const protocol_t::address_t baudrate = 8;
                typedef uint8_t baudrate_t;
                static const protocol_t::address_t return_delay_time = 9;
                typedef uint8_t return_delay_time_t;
                static const protocol_t::address_t drive_mode = 10;
                typedef uint8_t drive_mode_t;
                static const protocol_t::address_t operating_mode = 11;
                typedef uint8_t operating_mode_t;
                static const protocol_t::address_t secondary_id = 12;
                typedef uint8_t secondary_id_t;
                static const protocol_t::address_t protocol_version = 13;
                typedef uint8_t protocol_version_t;
                static const protocol_t::address_t homing_offset = 20;
                typedef uint32_t homing_offset_t;
                static const protocol_t::address_t moving_threshold = 24;
                typedef uint32_t moving_threshold_t;
                static const protocol_t::address_t temperature_limit = 31;
                typedef uint8_t temperature_limit_t;
                static const protocol_t::address_t max_voltage_limit = 32;
                typedef uint16_t max_voltage_limit_t;
                static const protocol_t::address_t min_voltage_limit = 34;
                typedef uint16_t min_voltage_limit_t;
                static const protocol_t::address_t pwm_limit = 36;
                typedef uint16_t pwm_limit_t;
                static const protocol_t::address_t velocity_limit = 44;
                typedef uint32_t velocity_limit_t;
                static const protocol_t::address_t max_position_limit = 48;
                typedef uint32_t max_position_limit_t;
                static const protocol_t::address_t min_position_limit = 52;
                typedef uint32_t min_position_limit_t;
                static const protocol_t::address_t shutdown = 63;
                typedef uint8_t shutdown_t;
                static const shutdown_t shutdown_value = 52;
                static const protocol_t::address_t torque_enable = 64;
                typedef uint8_t torque_enable_t;
                static const protocol_t::address_t led = 65;
                typedef uint8_t led_t;
                static const protocol_t::address_t status_return_level = 68;
                typedef uint8_t status_return_level_t;
                static const protocol_t::address_t registered_instruction = 69;
                typedef uint8_t registered_instruction_t;
                static const protocol_t::address_t hardware_error_status = 70;
                typedef uint8_t hardware_error_status_t;
                static const protocol_t::address_t velocity_i_gain = 76;
                typedef uint16_t velocity_i_gain_t;
                static const protocol_t::address_t velocity_p_gain = 78;
                typedef uint16_t velocity_p_gain_t;
                static const protocol_t::address_t position_d_gain = 80;
                typedef uint16_t position_d_gain_t;
                static const protocol_t::address_t position_i_gain = 82;
                typedef uint16_t position_i_gain_t;
                static const protocol_t::address_t position_p_gain = 84;
                typedef uint16_t position_p_gain_t;
                static const protocol_t::address_t feedforward_2nd_gain = 88;
                typedef uint16_t feedforward_2nd_gain_t;
                static const protocol_t::address_t feedforward_1st_gain = 90;
                typedef uint16_t feedforward_1st_gain_t;
                static const protocol_t::address_t bus_watchdog = 98;
                typedef uint8_t bus_watchdog_t;
                static const protocol_t::address_t goal_pwm = 100;
                typedef uint16_t goal_pwm_t;
                static const protocol_t::address_t goal_velocity = 104;
                typedef uint32_t goal_velocity_t;
                static const protocol_t::address_t profile_acceleration = 108;
                typedef uint32_t profil_acceleration_t;
                static const protocol_t::address_t profile_velocity = 112;
                typedef uint32_t profil_velocity_t;
                static const protocol_t::address_t goal_position = 116;
                typedef uint32_t goal_position_t;
                static const protocol_t::address_t realtime_tick = 120;
                typedef uint16_t realtime_tick_t;
                static const protocol_t::address_t moving = 122;
                typedef uint8_t moving_t;
                static const protocol_t::address_t moving_status = 123;
                typedef uint8_t moving_status_t;
                static const protocol_t::address_t present_pwm = 124;
                typedef uint16_t present_pwm_t;
                static const protocol_t::address_t present_load = 126;
                typedef uint16_t present_load_t;
                static const protocol_t::address_t present_velocity = 128;
                typedef uint32_t present_veclocity_t;
                static const protocol_t::address_t present_position = 132;
                typedef uint32_t present_position_t;
                static const protocol_t::address_t velocity_trajectory = 136;
                typedef uint32_t velocity_trajectory_t;
                static const protocol_t::address_t position_trajectory = 140;
                typedef uint32_t position_trajectory_t;
                static const protocol_t::address_t present_input_voltage = 144;
                typedef uint16_t present_input_voltage_t;
                static const protocol_t::address_t present_temperature = 146;
                typedef uint16_t present_temperature_t;
            };
        };

        class Xl430W250 : public Servo<Xl430W250> {
        public:
            typedef Xl430W250 Model;

            Xl430W250(typename protocol_t::id_t id) : Servo<Xl430W250>(id){};

            MODEL_NAME(Xl430W250);

            // Here we add the fields that are not common to all dynamixels
            READ_WRITE_FIELD(drive_mode);
            READ_WRITE_FIELD(operating_mode);
            READ_WRITE_FIELD(secondary_id);
            READ_WRITE_FIELD(protocol_version);
            READ_WRITE_FIELD(homing_offset);
            READ_WRITE_FIELD(moving_threshold);
            READ_WRITE_FIELD(temperature_limit);
            READ_WRITE_FIELD(max_voltage_limit);
            READ_WRITE_FIELD(min_voltage_limit);
            READ_WRITE_FIELD(pwm_limit);
            READ_WRITE_FIELD(velocity_limit);
            READ_WRITE_FIELD(max_position_limit);
            READ_WRITE_FIELD(min_position_limit);
            READ_WRITE_FIELD(shutdown);
            READ_WRITE_FIELD(velocity_i_gain);
            READ_WRITE_FIELD(velocity_p_gain);
            READ_WRITE_FIELD(position_d_gain);
            READ_WRITE_FIELD(position_i_gain);
            READ_WRITE_FIELD(protocol_version);
            READ_WRITE_FIELD(led);
            READ_WRITE_FIELD(position_d_gain);
            READ_WRITE_FIELD(position_i_gain);
            READ_WRITE_FIELD(position_p_gain);
            READ_WRITE_FIELD(feedforward_2nd_gain);
            READ_WRITE_FIELD(feedforward_1st_gain);
            READ_WRITE_FIELD(bus_watchdog);
            READ_WRITE_FIELD(goal_pwm);
            READ_WRITE_FIELD(goal_velocity);
            READ_WRITE_FIELD(profile_acceleration);
            READ_WRITE_FIELD(profile_velocity);
            READ_FIELD(model_information);
            READ_FIELD(registered_instruction);
            READ_FIELD(hardware_error_status);
            READ_FIELD(realtime_tick);
            READ_FIELD(moving_status);
            READ_FIELD(present_pwm);
            READ_FIELD(present_load);
            READ_FIELD(present_velocity);
            READ_FIELD(velocity_trajectory);
            READ_FIELD(position_trajectory);
            READ_FIELD(present_input_voltage);
            READ_FIELD(present_temperature);
        };
    } // namespace servos
} // namespace dynamixel

#endif
