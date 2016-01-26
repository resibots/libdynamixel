#ifndef DYNAMIXEL_MX106_HPP_
#define DYNAMIXEL_MX106_HPP_

#include <stdint.h>

#include "servo_model.hpp"
#include "../protocol1.hpp"

namespace dynamixel {
namespace models {
    class Mx106;

    template<>
    struct ModelTraits<Mx106> {
        typedef Protocol1 protocol_t;

        struct CT {
            static const protocol_t::address_t model_number = 0;
            typedef uint16_t model_number_t;
            static const model_number_t model_number_value = 356;
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
            static const protocol_t::address_t drive_mode = 10;
            typedef uint8_t drive_mode_t;
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
            static const protocol_t::address_t multi_turn_offset = 20;
            typedef uint16_t multi_turn_offset_t;
            static const protocol_t::address_t resolution_divider = 22;
            typedef uint8_t resolution_divider_t;
            static const protocol_t::address_t torque_enable = 24;
            typedef uint8_t torque_enable_t;
            static const protocol_t::address_t led = 25;
            typedef uint8_t led_t;
             static const led_t led_off = 0;
            static const led_t led_on = 1;
            static const protocol_t::address_t d_gain = 26;
            typedef uint8_t d_gain_t;
            static const protocol_t::address_t i_gain = 27;
            typedef uint8_t i_gain_t;
            static const protocol_t::address_t p_gain = 28;
            typedef uint8_t p_gain_t;
            static const protocol_t::address_t goal_position = 30;
            typedef uint16_t goal_position_t;
            static const goal_position_t min_goal_position = 0;
            static const goal_position_t max_goal_position = 4095;
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
            static const protocol_t::address_t current = 68;
            typedef uint16_t current_t;
            static const protocol_t::address_t torque_control_mode_enabled = 70;
            typedef uint8_t torque_control_mode_enabled_t;
            static const torque_control_mode_enabled_t torque_control_mode_enabled_off = 0;
            static const torque_control_mode_enabled_t torque_control_mode_enabled_on = 1;
            static const protocol_t::address_t goal_torque = 71;
            typedef uint16_t goal_torque_t;
            static const protocol_t::address_t goal_acceleration = 73;
            typedef uint8_t goal_acceleration_t;
        };
    };

    class Mx106 : public ServoModel<Mx106>,
    public AngleLimitServoModel<Mx106>,
    public DriveModeServoModel<Ex106>,
    public MaxTorqueServoModel<Mx106>,
    public AlarmLedServoModel<Mx106>,
    public MultiTurnOffsetServoModel<Mx106>,
    public ResolutionDividerServoModel<Mx106>,
    public SingleLedServoModel<Mx106>,
    public PidServoModel<Mx106>,
    public TorqueLimitServoModel<Mx106>,
    public PresentLoadServoModel<Mx106>,
    public LockServoModel<Mx106>,
    public PunchServoModel<Mx106>,
    public CurrentServoModel<Mx106>,
    public TorqueControlModeServoModel<Mx106>,
    public GoalTorqueServoModel<Mx106>,
    public GoalAccelerationServoModel<Mx106> {    
    };
}
}

#endif
