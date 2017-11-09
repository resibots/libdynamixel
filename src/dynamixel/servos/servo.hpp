#ifndef DYNAMIXEL_SERVOS_SERVO_HPP_
#define DYNAMIXEL_SERVOS_SERVO_HPP_

#include <cassert>
#include <stdint.h>

#include "../instruction_packet.hpp"
#include "../status_packet.hpp"
#include "base_servo.hpp"
#include "protocol_specific_packets.hpp"
#include "../instructions/ping.hpp"
#include "../instructions/read.hpp"
#include "../instructions/write.hpp"
#include "../instructions/reg_write.hpp"
#include "../instructions/action.hpp"
#include "../instructions/factory_reset.hpp"
#include "../instructions/reboot.hpp"
#include "../instructions/sync_write.hpp"
#include "model_traits.hpp"
#include "../errors/error.hpp"
#include "../errors/servo_limit_error.hpp"

#define MODEL_NAME(Name)                    \
    std::string model_name() const override \
    {                                       \
        return #Name;                       \
    }

#define READ_FIELD(Name)                                                                                                                                             \
    static InstructionPacket<typename Servo<Model>::protocol_t> get_##Name(typename Servo<Model>::protocol_t::id_t id)                                               \
    {                                                                                                                                                                \
        return typename Servo<Model>::read_t(id, Servo<Model>::ct_t::Name, sizeof(typename Servo<Model>::ct_t::Name##_t));                                           \
    }                                                                                                                                                                \
                                                                                                                                                                     \
    static typename Servo<Model>::ct_t::Name##_t parse_##Name(typename Servo<Model>::protocol_t::id_t id, const StatusPacket<typename Servo<Model>::protocol_t>& st) \
    {                                                                                                                                                                \
        typename Servo<Model>::ct_t::Name##_t res;                                                                                                                   \
        Servo<Model>::protocol_t::unpack_data(st.parameters(), res);                                                                                                 \
        return res;                                                                                                                                                  \
    }                                                                                                                                                                \
                                                                                                                                                                     \
    InstructionPacket<typename Servo<Model>::protocol_t> get_##Name() const override                                                                                 \
    {                                                                                                                                                                \
        return Model::get_##Name(this->_id);                                                                                                                         \
    }                                                                                                                                                                \
                                                                                                                                                                     \
    long long int parse_##Name(const StatusPacket<typename Servo<Model>::protocol_t>& st) const override                                                             \
    {                                                                                                                                                                \
        return static_cast<long long int>(Model::parse_##Name(this->_id, st));                                                                                       \
    }

#define WRITE_FIELD(Name)                                                                                                                                                  \
    static inline InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(typename Servo<Model>::protocol_t::id_t id, typename Servo<Model>::ct_t::Name##_t value) \
    {                                                                                                                                                                      \
        return typename Servo<Model>::write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                                   \
    }                                                                                                                                                                      \
                                                                                                                                                                           \
    static inline InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(typename Servo<Model>::protocol_t::id_t id, typename Servo<Model>::ct_t::Name##_t value) \
    {                                                                                                                                                                      \
        return typename Servo<Model>::reg_write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                               \
    }                                                                                                                                                                      \
                                                                                                                                                                           \
    inline InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(typename Servo<Model>::ct_t::Name##_t value) const                                              \
    {                                                                                                                                                                      \
        return Model::set_##Name(this->_id, value);                                                                                                                        \
    }                                                                                                                                                                      \
                                                                                                                                                                           \
    inline InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(typename Servo<Model>::ct_t::Name##_t value) const                                              \
    {                                                                                                                                                                      \
        return Model::reg_##Name(this->_id, value);                                                                                                                        \
    }                                                                                                                                                                      \
                                                                                                                                                                           \
    inline InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(long long int value) const override                                                             \
    {                                                                                                                                                                      \
        return this->set_##Name(static_cast<typename Servo<Model>::ct_t::Name##_t>(value));                                                                                \
    }                                                                                                                                                                      \
                                                                                                                                                                           \
    inline InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(long long int value) const override                                                             \
    {                                                                                                                                                                      \
        return this->reg_##Name(static_cast<typename Servo<Model>::ct_t::Name##_t>(value));                                                                                \
    }

#define READ_WRITE_FIELD(Name) \
    READ_FIELD(Name)           \
    WRITE_FIELD(Name)

namespace dynamixel {
    namespace servos {

        template <class Model>
        class Servo : public BaseServo<typename ModelTraits<Model>::protocol_t> {
        public:
            typedef typename ModelTraits<Model>::protocol_t protocol_t;
            typedef typename ModelTraits<Model>::CT ct_t;
            typedef instructions::Ping<protocol_t> ping_t;
            typedef instructions::Read<protocol_t> read_t;
            typedef instructions::Write<protocol_t> write_t;
            typedef instructions::RegWrite<protocol_t> reg_write_t;
            typedef instructions::Action<protocol_t> action_t;
            typedef instructions::FactoryReset<protocol_t> factory_reset_t;
            typedef instructions::SyncWrite<protocol_t> sync_write_t;

            long long int id() const override
            {
                return this->_id;
            }

            void id(long long int id) override
            {
                this->_id = static_cast<typename protocol_t::id_t>(id);
            }

            READ_FIELD(model_number);
            READ_FIELD(firmware_version);
            READ_WRITE_FIELD(id);
            READ_WRITE_FIELD(baudrate);
            READ_WRITE_FIELD(return_delay_time);
            READ_WRITE_FIELD(highest_temperature_limit);
            READ_WRITE_FIELD(highest_voltage_limit);
            READ_WRITE_FIELD(lowest_voltage_limit);
            READ_WRITE_FIELD(status_return_level);
            READ_WRITE_FIELD(alarm_shutdown);
            READ_WRITE_FIELD(torque_enable);
            READ_WRITE_FIELD(goal_position);
            READ_WRITE_FIELD(moving_speed);
            READ_FIELD(present_position);
            READ_FIELD(present_speed);
            READ_FIELD(present_voltage);
            READ_FIELD(present_temperature);
            READ_FIELD(registered);
            READ_FIELD(moving);

            static inline InstructionPacket<protocol_t> ping(typename Servo<Model>::protocol_t::id_t id)
            {
                return ping_t(id);
            }

            InstructionPacket<protocol_t> ping() const override
            {
                return ping_t(this->_id);
            }

            // =================================================================
            // Position-specific

            static inline InstructionPacket<protocol_t> set_goal_position_angle(typename Servo<Model>::protocol_t::id_t id, double rad)
            {
                double deg = rad * 57.2958;
                if (!(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg))
                    throw errors::ServoLimitError(
                        id,
                        ct_t::min_goal_angle_deg * 0.01745, // convert from deg to rad
                        ct_t::max_goal_angle_deg * 0.01745,
                        rad
                        );
                typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
                return set_goal_position(id, pos);
            }

            static inline InstructionPacket<protocol_t> reg_goal_position_angle(typename Servo<Model>::protocol_t::id_t id, double rad)
            {
                double deg = rad * 57.2958;
                if (!(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg))
                    throw errors::ServoLimitError(
                        id,
                        ct_t::min_goal_angle_deg * 0.01745,
                        ct_t::max_goal_angle_deg * 0.01745,
                        rad
                        );
                typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
                return reg_goal_position(id, pos);
            }

            InstructionPacket<protocol_t> set_goal_position_angle(double rad) const override
            {
                return Model::set_goal_position_angle(this->_id, rad);
            }

            InstructionPacket<protocol_t> reg_goal_position_angle(double rad) const override
            {
                return Model::reg_goal_position_angle(this->_id, rad);
            }

            static InstructionPacket<typename Servo<Model>::protocol_t> get_present_position_angle(typename Servo<Model>::protocol_t::id_t id)
            {
                return get_present_position(id);
            }

            InstructionPacket<typename Servo<Model>::protocol_t> get_present_position_angle() const override
            {
                return Model::get_present_position_angle(this->_id);
            }

            static double parse_present_position_angle(typename Servo<Model>::protocol_t::id_t id, const StatusPacket<typename Servo<Model>::protocol_t>& st)
            {
                typename Servo<Model>::ct_t::present_position_t pos;
                Servo<Model>::protocol_t::unpack_data(st.parameters(), pos);
                double deg = ((pos - ct_t::min_goal_position) * (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg) / (ct_t::max_goal_position - ct_t::min_goal_position)) + ct_t::min_goal_angle_deg;
                double rad = deg / 57.2958;
                return rad;
            }

            double parse_present_position_angle(const StatusPacket<typename Servo<Model>::protocol_t>& st) const override
            {
                return Model::parse_present_position_angle(this->_id, st);
            }

            // Bulk operations. Only works if the models are known and they are all the same
            template <typename Id, typename Pos>
            static InstructionPacket<protocol_t> set_goal_positions(const std::vector<Id>& ids, const std::vector<Pos>& pos)
            {
                if (ids.size() != pos.size())
                    throw errors::Error("Instruction: error when setting goal positions: \n\tMismatch in vector size for ids and positions");
                std::vector<std::vector<uint8_t>> packed(pos.size());
                for (size_t i = 0; i < pos.size(); i++)
                    packed[i] = protocol_t::pack_data((typename ct_t::goal_position_t)pos[i]);

                return sync_write_t(ct_t::goal_position, _get_typed<typename protocol_t::id_t>(ids), packed);
            }

            // =================================================================
            // Speed-specific

            static inline InstructionPacket<protocol_t> set_moving_speed_angle(typename Servo<Model>::protocol_t::id_t id, double rad_per_s, cst::OperatingMode operating_mode)
            {
                return ProtocolSpecificPackets<Model, protocol_t>::set_moving_speed_angle(id, rad_per_s, operating_mode);
            }

            static inline InstructionPacket<protocol_t> reg_moving_speed_angle(typename Servo<Model>::protocol_t::id_t id, double rad_per_s, cst::OperatingMode operating_mode)
            {
                return ProtocolSpecificPackets<Model, protocol_t>::reg_moving_speed_angle(id, rad_per_s, operating_mode);
            }

            InstructionPacket<protocol_t> set_moving_speed_angle(double rad_per_s, cst::OperatingMode operating_mode = cst::joint) const override
            {
                return Model::set_moving_speed_angle(this->_id, rad_per_s, operating_mode);
            }

            InstructionPacket<protocol_t> reg_moving_speed_angle(double rad_per_s, cst::OperatingMode operating_mode = cst::joint) const override
            {
                return Model::reg_moving_speed_angle(this->_id, rad_per_s, operating_mode);
            }

            // TODO: read speed from dynamixel pros to check that we do get negative values too
            // FIXME : replace the following by protocol specific methods ?
            static double parse_joint_speed(typename Servo<Model>::protocol_t::id_t id, const StatusPacket<typename Servo<Model>::protocol_t>& st)
            {
                typename Servo<Model>::ct_t::present_speed_t speed;
                Servo<Model>::protocol_t::unpack_data(st.parameters(), speed);

                int8_t sign;
                if (Servo<Model>::ct_t::speed_sign_bit) { // the highest bit is used for the sign
                    sign = speed / Servo<Model>::ct_t::max_goal_speed == 0 ? 1 : -1;
                    speed = speed % Servo<Model>::ct_t::max_goal_speed;
                }
                else {
                    sign = 1;
                }

                double speed_rpm = sign * speed * Servo<Model>::ct_t::rpm_per_tick;
                double speed_si = speed_rpm * 0.104720; // convertion ratio is 2*pi/60
                return speed_si;
            }

            double parse_joint_speed(const StatusPacket<typename Servo<Model>::protocol_t>& st) const override
            {
                return Model::parse_joint_speed(this->_id, st);
            }

            template <typename Id, typename Speed>
            static InstructionPacket<protocol_t> set_moving_speeds(const std::vector<Id>& ids, const std::vector<Speed>& speeds)
            {
                if (ids.size() != speeds.size())
                    throw errors::Error("Instruction: error when setting moving speeds: \n\tMismatch in vector size for ids and speeds");
                std::vector<std::vector<uint8_t>> packed(speeds.size());
                for (size_t i = 0; i < speeds.size(); i++)
                    packed[i] = protocol_t::pack_data((typename ct_t::moving_speed_t)speeds[i]);

                return sync_write_t(ct_t::moving_speed, _get_typed<typename protocol_t::id_t>(ids), packed);
            }

            // =================================================================
            // Torque-specific

            template <typename Id, typename TorqueLimit>
            static InstructionPacket<protocol_t> set_torque_limits(const std::vector<Id>& ids, const std::vector<TorqueLimit>& torque_limits)
            {
                if (ids.size() != torque_limits.size())
                    throw errors::Error("Instruction: error when setting torque limits: \n\tMismatch in vector size for ids and torques");
                std::vector<std::vector<uint8_t>> packed(torque_limits.size());
                for (size_t i = 0; i < torque_limits.size(); i++)
                    packed[i] = protocol_t::pack_data((typename ct_t::torque_limit_t)torque_limits[i]);

                return sync_write_t(ct_t::torque_limit, _get_typed<typename protocol_t::id_t>(ids), packed);
            }

        protected:
            Servo(typename protocol_t::id_t id) : _id(id) {}

            template <typename T, typename U>
            static std::vector<T> _get_typed(std::vector<U> vector)
            {
                std::vector<T> result(vector.size());
                for (size_t i = 0; i < vector.size(); i++)
                    result[i] = (T)vector[i];

                return result;
            }

            typename protocol_t::id_t _id;
        };
    }
}

#endif
