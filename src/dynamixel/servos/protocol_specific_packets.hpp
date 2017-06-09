#ifndef DYNAMIXEL_SERVOS_PROTOSPECIFICPACKETS_HPP_
#define DYNAMIXEL_SERVOS_PROTOSPECIFICPACKETS_HPP_

#include <cmath>

#include "../instruction_packet.hpp"
#include "../protocols.hpp"
#include "base_servo.hpp"
#include "model_traits.hpp"
#include "../errors/error.hpp"
#include "../errors/servo_limit_error.hpp"

namespace dynamixel {
    namespace servos {

        template <class Model>
        class Servo;

        template <class M, class P>
        class ProtocolSpecificPackets {
        public:
            typedef typename ModelTraits<M>::CT ct_t;

            /** Build a packet to set the desired speed on an actuator.

                See the protocol-specific implemetnations for details.

                @param id identifier of the actuator
                @param rad_per_s rotational speed, in radians per second
                @param operating_mode (enum) mode in which the actuator is
                    controlled; for version 1 of the protocol, it has an impact
                    on how the values are treated
                @return a data packet to be sent on the serial line
            **/
            static inline InstructionPacket<P> set_moving_speed_angle(
                typename P::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode = cst::joint)
            {
                throw errors::Error("set_moving_speed_angle not implemented for this protocol");
            }

            /** Build a packet to set the desired speed on an actuator (in register).

                See the protocol-specific implemetnations for details.

                @param id identifier of the actuator
                @param rad_per_s rotational speed, in radians per second
                @param operating_mode (enum) mode in which the actuator is
                    controlled; for version 1 of the protocol, it has an impact
                    on how the values are treated
                @return a data packet to be sent on the serial line
            **/
            static inline InstructionPacket<P> reg_moving_speed_angle(
                typename P::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode = cst::joint)
            {
                throw errors::Error("reg_moving_speed_angle not implemented for this protocol");
            }
        };

        template <class M>
        class ProtocolSpecificPackets<M, protocols::Protocol1> {
        public:
            typedef typename ModelTraits<M>::CT ct_t;
            typedef typename ct_t::moving_speed_t moving_speed_t;

            static inline InstructionPacket<protocols::Protocol1> set_moving_speed_angle(
                typename protocols::Protocol1::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode)
            {
                moving_speed_t speed_ticks = angular_speed_to_ticks(id, rad_per_s,
                    operating_mode);

                return Servo<M>::set_moving_speed(id, speed_ticks);
            }

            static inline InstructionPacket<protocols::Protocol1> reg_moving_speed_angle(
                typename protocols::Protocol1::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode)
            {
                moving_speed_t speed_ticks = angular_speed_to_ticks(id, rad_per_s,
                    operating_mode);

                return Servo<M>::reg_moving_speed(id, speed_ticks);
            }

        private:
            // 2 * pi
            static constexpr double two_pi = 6.28318;

            static inline moving_speed_t angular_speed_to_ticks(
                typename protocols::Protocol1::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode)
            {
                // convert radians per second to ticks
                int32_t speed_ticks = round(60 * rad_per_s / (two_pi * ct_t::rpm_per_tick));

                // The actuator is operated as a wheel (continuous rotation)
                if (operating_mode == cst::wheel) {
                    // Check that desired speed is within the actuator's bounds
                    if (!(abs(speed_ticks) >= ct_t::min_goal_speed && abs(speed_ticks) <= ct_t::max_goal_speed)) {
                        double min_goal_speed = -ct_t::max_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                        double max_goal_speed = ct_t::max_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                        throw errors::ServoLimitError(id, min_goal_speed, max_goal_speed, rad_per_s, "speed (rad/s)");
                    }

                    // Move negatives values in the range [ct_t::min_goal_speed,
                    // ct_t::2*max_goal_speed+1]
                    if (speed_ticks < 0) {
                        speed_ticks = -speed_ticks + ct_t::max_goal_speed + 1;
                    }
                }
                // The actuator is operated as a joint (not continuous rotation)
                else if (operating_mode == cst::joint) {
                    if (!(speed_ticks >= ct_t::min_goal_speed && speed_ticks <= ct_t::max_goal_speed)) {
                        double min_goal_speed = ct_t::min_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                        double max_goal_speed = ct_t::max_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                        throw errors::ServoLimitError(id, min_goal_speed, max_goal_speed, rad_per_s, "speed (rad/s)");
                    }
                }

                return (moving_speed_t)speed_ticks;
            }
        };

        template <class M>
        class ProtocolSpecificPackets<M, protocols::Protocol2> {
        public:
            typedef typename ModelTraits<M>::CT ct_t;
            typedef typename ct_t::moving_speed_t moving_speed_t;

            static inline InstructionPacket<protocols::Protocol2> set_moving_speed_angle(
                typename protocols::Protocol2::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode)
            {
                moving_speed_t speed_ticks = angular_speed_to_ticks(id, rad_per_s);

                return Servo<M>::set_moving_speed(id, speed_ticks);
            }

            static inline InstructionPacket<protocols::Protocol2> reg_moving_speed_angle(
                typename protocols::Protocol2::id_t id,
                double rad_per_s,
                cst::OperatingMode operating_mode)
            {
                moving_speed_t speed_ticks = angular_speed_to_ticks(id, rad_per_s);

                return Servo<M>::reg_moving_speed(id, speed_ticks);
            }

        private:
            // 2 * pi
            static constexpr double two_pi = 6.28318;

            static inline moving_speed_t angular_speed_to_ticks(
                typename protocols::Protocol2::id_t id,
                double rad_per_s)
            {
                // convert radians per second to ticks
                int32_t speed_ticks = round(60 * rad_per_s / (two_pi * ct_t::rpm_per_tick));

                // Check that desired speed is within the actuator's bounds
                if (!(abs(speed_ticks) <= ct_t::max_goal_speed)) {
                    double min_goal_speed = ct_t::min_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                    double max_goal_speed = ct_t::max_goal_speed * ct_t::rpm_per_tick * two_pi / 60;
                    throw errors::ServoLimitError(id, min_goal_speed, max_goal_speed, rad_per_s, "speed (rad/s)");
                }

                return (moving_speed_t)speed_ticks;
            }
        };
    }
}

#endif // DYNAMIXEL_SERVOS_PROTOSPECIFICPACKETS_HPP_
