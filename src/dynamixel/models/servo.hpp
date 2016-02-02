#ifndef DYNAMIXEL_MODELS_SERVO_HPP_
#define DYNAMIXEL_MODELS_SERVO_HPP_

#include <cassert>
#include <cmath>
#include <stdint.h>

#include "../instruction_packet.hpp"
#include "../status_packet.hpp"
#include "../instructions/ping.hpp"
#include "../instructions/read.hpp"
#include "../instructions/write.hpp"
#include "../instructions/reg_write.hpp"
#include "../instructions/action.hpp"
#include "../instructions/factory_reset.hpp"
#include "../instructions/sync_write.hpp"
#include "model_traits.hpp"

#define READ_FIELD(Name) \
    static InstructionPacket<typename Servo<Model>::protocol_t> get_##Name(typename Servo<Model>::protocol_t::id_t id)                 \
    {                                                                                                                                                                                                                                              \
        return typename Servo<Model>::read_t(id, Servo<Model>::ct_t::Name, sizeof(typename Servo<Model>::ct_t::Name##_t));   \
    }                                                                                                                                                                                                                                              \
                                                                                                                                                                                                                                                    \
    InstructionPacket<typename Servo<Model>::protocol_t> get_##Name() const                                                                                                    \
    {                                                                                                                                                                                                                                              \
        return typename Servo<Model>::read_t(this->_id, Servo<Model>::ct_t::Name, sizeof(typename Servo<Model>::ct_t::Name##_t));   \
    }                                                                                                                                                                                                                                              \
                                                                                                                                                                                                                                                    \
    static typename Servo<Model>::ct_t::Name##_t  parse_##Name(const StatusPacket<typename Servo<Model>::protocol_t>& st) \
    {                                                                                                                                                                                                                                               \
        typename Servo<Model>::ct_t::Name##_t res;                                                                                                                                                      \
        Servo<Model>::protocol_t::unpack_data(st.parameters(), res);                                                                                                             \
        return res;                                                                                                                                                                                                                              \
    }                                                                                                                                                                                                                               

#define WRITE_FIELD(Name) \
    static InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(typename Servo<Model>::protocol_t::id_t id, typename Servo<Model>::ct_t::Name##_t value)      \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                                                                      \
    }                                                                                                                                                                                                                             \
                                                                                                                                                                                                                                    \
    static InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(typename Servo<Model>::protocol_t::id_t id, typename Servo<Model>::ct_t::Name##_t value)      \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::reg_write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                                                               \
    }                                                                                                                                                                                                                             \
                                                                                                                                                                                                                                    \
    InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(typename Servo<Model>::ct_t::Name##_t value)  const \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::write_t(this->_id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                                                          \
    }                                                                                                                                                                                                                             \
                                                                                                                                                                                                                                    \
    InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(typename Servo<Model>::ct_t::Name##_t value)      \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::reg_write_t(this->_id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value));                                                                               \
    }

#define WRITE_BOOL_FIELD(Name, Bool_Name) \
    static InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(typename Servo<Model>::protocol_t::id_t id, bool value)  \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value ? Servo<Model>::ct_t::Bool_Name##_on : Servo<Model>::ct_t::Bool_Name##_off)); \
    }                                                                                                                                                                                                                             \
                                                                                                                                                                                                                                    \
    static InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(typename Servo<Model>::protocol_t::id_t id, bool value)  \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::reg_write_t(id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value ? Servo<Model>::ct_t::Bool_Name##_on : Servo<Model>::ct_t::Bool_Name##_off)); \
    }                                                                                                                                                                                                                             \
                                                                                                                                                                                                                                    \
    InstructionPacket<typename Servo<Model>::protocol_t> set_##Name(bool value)  \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::write_t(this->_id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value ? Servo<Model>::ct_t::Bool_Name##_on : Servo<Model>::ct_t::Bool_Name##_off));                                                                                                       \
    }                                                                                                                                                                                                                            \
                                                                                                                                                                                                                                  \
    InstructionPacket<typename Servo<Model>::protocol_t> reg_##Name(bool value)  \
    {                                                                                                                                                                                                                           \
            return typename Servo<Model>::reg_write_t(this->_id, Servo<Model>::ct_t::Name, Servo<Model>::protocol_t::pack_data(value ? Servo<Model>::ct_t::Bool_Name##_on : Servo<Model>::ct_t::Bool_Name##_off)); \
    }

#define READ_WRITE_FIELD(Name) READ_FIELD(Name) \
    WRITE_FIELD(Name)

namespace dynamixel {
namespace models {
    template <class Model>
    class Servo {
    public:
        typedef ModelTraits<Model> traits_t;
        typedef typename traits_t::protocol_t protocol_t;
        typedef typename traits_t::CT ct_t;
        typedef instructions::Ping<protocol_t> ping_t;
        typedef instructions::Read<protocol_t> read_t;
        typedef instructions::Write<protocol_t> write_t;
        typedef instructions::RegWrite<protocol_t> reg_write_t;
        typedef instructions::Action<protocol_t> action_t;
        typedef instructions::FactoryReset<protocol_t> factory_reset_t;
        typedef instructions::SyncWrite<protocol_t> sync_write_t;

        Servo(typename protocol_t::id_t id) : _id(id) {}

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

        static InstructionPacket<protocol_t> ping(typename Servo<Model>::protocol_t::id_t id)
        {
            return ping_t(id);
        }

        InstructionPacket<protocol_t> ping()
        {
            return ping_t(this->_id);
        }

        static InstructionPacket<protocol_t> set_goal_position_angle(typename Servo<Model>::protocol_t::id_t id, double deg)
        {
            assert(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg);
            typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
            return set_goal_position(id, pos);
        }

        static InstructionPacket<protocol_t> reg_goal_position_angle(typename Servo<Model>::protocol_t::id_t id, double deg)
        {
            assert(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg);
            typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
            return reg_goal_position(id, pos);
        }

        InstructionPacket<protocol_t> set_goal_position_angle(double deg)
        {
            assert(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg);
            typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
            return set_goal_position(this->_id, pos);
        }

        InstructionPacket<protocol_t> reg_goal_position_angle(double deg)
        {
            assert(deg >= ct_t::min_goal_angle_deg && deg <= ct_t::max_goal_angle_deg);
            typename ct_t::goal_position_t pos = ((deg - ct_t::min_goal_angle_deg) * (ct_t::max_goal_position - ct_t::min_goal_position) / (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg)) + ct_t::min_goal_position;
            return reg_goal_position(this->_id, pos);
        }

        static InstructionPacket<typename Servo<Model>::protocol_t> get_present_position_angle(typename Servo<Model>::protocol_t::id_t id)
        {
            return get_present_position(id);
        }

        static double parse_present_position_angle(const StatusPacket<typename Servo<Model>::protocol_t>& st)
        {
            typename Servo<Model>::ct_t::present_position_t pos;
            Servo<Model>::protocol_t::unpack_data(st.parameters(), pos);
            double res = ((pos - ct_t::min_goal_position) * (ct_t::max_goal_angle_deg - ct_t::min_goal_angle_deg) / (ct_t::max_goal_position - ct_t::min_goal_position)) + ct_t::min_goal_angle_deg;
            return res;
        }

        template <typename Id, typename Pos>
        static InstructionPacket<protocol_t> set_goal_positions(const std::vector<Id>& ids, const std::vector<Pos>& pos)
        {
            assert(ids.size() == pos.size());
            std::vector<std::vector<uint8_t> > packed(pos.size());
            for (size_t i = 0; i < pos.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::goal_position_t)pos[i]);

            return sync_write_t(ct_t::goal_position, get_typed<typename protocol_t::id_t>(ids), packed);
        }

        template <typename Id, typename Speed>
        static InstructionPacket<protocol_t> set_moving_speeds(const std::vector<Id>& ids, const std::vector<Speed>& speeds)
        {
            assert(ids.size() == speeds.size());
            std::vector<std::vector<uint8_t> > packed(speeds.size());
            for (size_t i = 0; i < speeds.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::moving_speed_t)speeds[i]);

            return sync_write_t(ct_t::moving_speed, get_typed<typename protocol_t::id_t>(ids), packed);
        }

        template <typename Id, typename TorqueLimit>
        static InstructionPacket<protocol_t> set_torque_limits(const std::vector<Id>& ids, const std::vector<TorqueLimit>& torque_limits)
        {
            assert(ids.size() == torque_limits.size());
            std::vector<std::vector<uint8_t> > packed(torque_limits.size());
            for (size_t i = 0; i < torque_limits.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::torque_limit_t)torque_limits[i]);

            return typename sync_write_t(ct_t::torque_limit, _get_typed<typename protocol_t::id_t>(ids), packed);
        }

    protected:
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
