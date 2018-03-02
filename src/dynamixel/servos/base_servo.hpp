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
    namespace servos {
        namespace cst {
            enum OperatingMode {
                wheel,
                joint,
                multi_turn
            };
        }

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
            BASE_FIELD(model_number);
            BASE_FIELD(firmware_version);
            BASE_FIELD(id);
            BASE_FIELD(baudrate);
            BASE_FIELD(return_delay_time);
            BASE_FIELD(highest_temperature_limit);
            BASE_FIELD(highest_voltage_limit);
            BASE_FIELD(lowest_voltage_limit);
            BASE_FIELD(status_return_level);
            BASE_FIELD(alarm_shutdown);
            BASE_FIELD(torque_enable);
            BASE_FIELD(goal_position);
            BASE_FIELD(moving_speed);
            BASE_FIELD(present_position);
            BASE_FIELD(present_speed);
            BASE_FIELD(present_voltage);
            BASE_FIELD(present_temperature);
            BASE_FIELD(registered);
            BASE_FIELD(moving);

            BASE_FIELD(cw_angle_limit);
            BASE_FIELD(ccw_angle_limit);
            BASE_FIELD(drive_mode);
            BASE_FIELD(max_torque);
            BASE_FIELD(alarm_led);
            BASE_FIELD(led);
            BASE_FIELD(cw_compliance_margin);
            BASE_FIELD(ccw_compliance_margin);
            BASE_FIELD(cw_compliance_slope);
            BASE_FIELD(ccw_compliance_slope);
            BASE_FIELD(torque_limit);
            BASE_FIELD(present_load);
            BASE_FIELD(lock);
            BASE_FIELD(punch);
            BASE_FIELD(sensed_current);

            BASE_FIELD(multi_turn_offset);
            BASE_FIELD(resolution_divider);
            BASE_FIELD(d_gain);
            BASE_FIELD(i_gain);
            BASE_FIELD(p_gain);
            BASE_FIELD(current);
            BASE_FIELD(torque_control_mode_enabled);
            BASE_FIELD(goal_torque);
            BASE_FIELD(goal_acceleration);

            BASE_FIELD(model_info);
            BASE_FIELD(operating_mode);
            BASE_FIELD(homing_offset);
            BASE_FIELD(moving_threshold);
            BASE_FIELD(acceleration_limit);
            BASE_FIELD(speed_limit);
            BASE_FIELD(max_position_limit);
            BASE_FIELD(min_position_limit);
            BASE_FIELD(led_r);
            BASE_FIELD(led_g);
            BASE_FIELD(led_b);
            BASE_FIELD(velocity_i_gain);
            BASE_FIELD(velocity_p_gain);
            BASE_FIELD(position_p_gain);
            BASE_FIELD(hardware_error_status);

            BASE_FIELD(control_mode);

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

            virtual InstructionPacket<protocol_t> set_moving_speed_angle(double rad_per_s, cst::OperatingMode operating_mode = cst::joint) const
            {
                throw errors::Error("set_moving_speed_angle not implemented in model");
            }

            virtual InstructionPacket<protocol_t> reg_moving_speed_angle(double rad_per_s, cst::OperatingMode operating_mode = cst::joint) const
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
