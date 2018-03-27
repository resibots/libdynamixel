#ifndef DYNAMIXEL_SERVOS_BASE_SERVO_HPP_
#define DYNAMIXEL_SERVOS_BASE_SERVO_HPP_

#include "../instruction_packet.hpp"
#include "../errors/error.hpp"

#define BASE_FIELD(Name)                                                         \
    virtual InstructionPacket<protocol_t> get_##Name() const                     \
    {                                                                            \
        throw errors::Error("get_" #Name " not implemented in model");           \
    }                                                                            \
                                                                                 \
    virtual long long int parse_##Name(const StatusPacket<protocol_t>& st) const \
    {                                                                            \
        throw errors::Error("parse_" #Name " not implemented in model");         \
    }                                                                            \
                                                                                 \
    virtual InstructionPacket<protocol_t> set_##Name(long long int value) const  \
    {                                                                            \
        throw errors::Error("set_" #Name " not implemented in model");           \
    }                                                                            \
                                                                                 \
    virtual InstructionPacket<protocol_t> reg_##Name(long long int value) const  \
    {                                                                            \
        throw errors::Error("reg_" #Name " not implemented in model");           \
    }

namespace dynamixel {
    enum class OperatingMode {
        torque,
        wheel,
        joint,
        multi_turn,
        unknown
    };
    namespace servos {

        template <typename Protocol>
        class BaseServo {
        public:
            typedef Protocol protocol_t;

            virtual long long int id() const
            {
                throw errors::Error("id not implemented in model");
            }

            virtual void id(long long int id)
            {
                throw errors::Error("id not implemented in model");
            }

            virtual std::string model_name() const
            {
                throw errors::Error("model_name not implemented in model");
            }

            // All the memory addresses of all the models need to be declared here
            // Then, the concrete model classes override the ones that they have

            // EEPROM
            // ======

            // Fields for models with protocol 1
            // ---------------------------------
            BASE_FIELD(model_number);
            BASE_FIELD(firmware_version);
            BASE_FIELD(id);
            BASE_FIELD(baudrate);
            BASE_FIELD(return_delay_time);
            BASE_FIELD(cw_angle_limit);
            BASE_FIELD(ccw_angle_limit);
            BASE_FIELD(drive_mode); // Only for MX-106 and EX-106+
            BASE_FIELD(highest_temperature_limit);
            BASE_FIELD(lowest_voltage_limit);
            BASE_FIELD(highest_voltage_limit);
            BASE_FIELD(max_torque);
            BASE_FIELD(alarm_led);
            BASE_FIELD(alarm_shutdown); // Corresponds to "shutdown" in protocol 2
            // Only for MX-series {
            BASE_FIELD(multi_turn_offset);
            BASE_FIELD(resolution_divider);
            // } Only for MX-series

            // Extra fields for models with protocol 2
            // ---------------------------------------
            BASE_FIELD(model_info);
            BASE_FIELD(operating_mode);
            BASE_FIELD(homing_offset); // Not for L42
            BASE_FIELD(moving_threshold);
            BASE_FIELD(acceleration_limit);
            // BASE_FIELD(torque_limit) // FIXME: This field is in RAM for protocol 1 devices and in EEPROM for the dynamixel Pro series. For the former, this field is replaced with max_torque
            BASE_FIELD(speed_limit);
            BASE_FIELD(max_position_limit); // FIXME: these two fields could be replaced with cw_angle_limit
            BASE_FIELD(min_position_limit); // and ccw_angle_limit
            // MX with protocol 2 only {
            BASE_FIELD(secondary_id);
            BASE_FIELD(protocol_version);
            BASE_FIELD(pwm_limit);
            BASE_FIELD(current_limit);
            // } MX with protocol 2 only

            // RAM
            // ===

            // Fields for models with protocol 1
            // ---------------------------------
            BASE_FIELD(torque_enable);
            BASE_FIELD(led);
            // Only protocol 1, L42 and MX protocol 2 {
            BASE_FIELD(position_d_gain);
            BASE_FIELD(position_i_gain);
            // } Only protocol 1, L42 and MX protocol 2
            BASE_FIELD(position_p_gain);
            // Only for AX series and EX-106+ {
            BASE_FIELD(cw_compliance_margin);
            BASE_FIELD(ccw_compliance_margin);
            BASE_FIELD(cw_compliance_slope);
            BASE_FIELD(ccw_compliance_slope);
            // } Only for AX series and EX-106+
            BASE_FIELD(goal_position);
            BASE_FIELD(moving_speed); // for protocol 2 means "goal_velocity"
            BASE_FIELD(torque_limit);
            BASE_FIELD(present_position);
            BASE_FIELD(present_speed);
            BASE_FIELD(present_load);
            BASE_FIELD(present_voltage);
            BASE_FIELD(present_temperature);
            BASE_FIELD(registered);
            BASE_FIELD(moving);
            BASE_FIELD(lock);
            BASE_FIELD(punch);
            // Only for MX-106 and MX-64 {
            // TODO: replace sensed_current and current with present_current everywhere
            BASE_FIELD(sensed_current); // Also EX-106+ and corresponds to "present_current" for protocol 2
            BASE_FIELD(torque_control_mode_enabled);
            BASE_FIELD(goal_torque);
            // } Only for MX-106 and MX-64
            BASE_FIELD(goal_acceleration); // Only for MX series

            // Extra fields for models with protocol 2
            // ---------------------------------------
            // Only MX protocol 2
            BASE_FIELD(status_return_level);
            BASE_FIELD(led_r);
            BASE_FIELD(led_g);
            BASE_FIELD(led_b);
            // Not for L42 {
            BASE_FIELD(velocity_i_gain);
            BASE_FIELD(velocity_p_gain);
            // } Not for L42
            BASE_FIELD(hardware_error_status);
            // Only MX with protocol 2 {
            BASE_FIELD(feedforward_2nd_gain);
            BASE_FIELD(feedforward_1st_gain);
            BASE_FIELD(bus_watchdog);
            BASE_FIELD(goal_pwm);
            BASE_FIELD(goal_current);
            BASE_FIELD(profile_acceleration);
            BASE_FIELD(profile_speed);
            BASE_FIELD(realtime_tick);
            BASE_FIELD(moving_status);
            BASE_FIELD(present_pwm);
            BASE_FIELD(speed_trajectory);
            BASE_FIELD(position_trajectory);
            // } Only MX with protocol 2

            BASE_FIELD(current); // FIXME: duplicate with sensed_current
            BASE_FIELD(control_mode); // FIXME: duplicate with operating_mode

            // Common functionality for the servos that will be implemented in the Servo class (where we know the model)
            virtual InstructionPacket<protocol_t> ping() const
            {
                throw errors::Error("ping not implemented in model");
            }

            // =================================================================
            // Position-specific

            virtual InstructionPacket<protocol_t> set_goal_position_angle(double rad) const
            {
                throw errors::Error("set_goal_position_angle not implemented in model");
            }

            virtual InstructionPacket<protocol_t> reg_goal_position_angle(double rad) const
            {
                throw errors::Error("reg_goal_position_angle not implemented in model");
            }

            virtual InstructionPacket<protocol_t> get_present_position_angle() const
            {
                throw errors::Error("get_present_position_angle not implemented in model");
            }

            virtual double parse_present_position_angle(const StatusPacket<protocol_t>& st) const
            {
                throw errors::Error("parse_present_position_angle not implemented in model");
            }

            // =================================================================
            // Speed-specific

            virtual InstructionPacket<protocol_t> set_moving_speed_angle(double rad_per_s, OperatingMode operating_mode = OperatingMode::joint) const
            {
                throw errors::Error("set_moving_speed_angle not implemented in model");
            }

            virtual InstructionPacket<protocol_t> reg_moving_speed_angle(double rad_per_s, OperatingMode operating_mode = OperatingMode::joint) const
            {
                throw errors::Error("reg_moving_speed_angle not implemented in model");
            }

            virtual double parse_joint_speed(const StatusPacket<protocol_t>& st) const
            {
                throw errors::Error("parse_joint_speed not implemented in model");
            }

        protected:
            BaseServo() {}
        };
    } // namespace servos
} // namespace dynamixel

#endif
