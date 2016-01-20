#ifndef DYNAMIXEL_MODELS_SERVO_MODEL_HPP_
#define DYNAMIXEL_MODELS_SERVO_MODEL_HPP_

#include <cassert>
#include <stdint.h>

#include "../instruction_packet.hpp"
#include "../instructions/ping.hpp"
#include "../instructions/read.hpp"
#include "../instructions/write.hpp"
#include "../instructions/reg_write.hpp"
#include "../instructions/action.hpp"
#include "../instructions/factory_reset.hpp"
#include "../instructions/sync_write.hpp"
#include "model_traits.hpp"

namespace dynamixel {
namespace models {
    template <class Model>
    class ServoModel {
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

        static InstructionPacket<protocol_t> set_id(typename protocol_t::id_t old_id, typename protocol_t::id_t new_id, bool reg = false)
        {
            if (reg)
                return reg_write_t(old_id, ct_t::id, protocol_t::pack_data(new_id));
            else
                return write_t(old_id, ct_t::id, protocol_t::pack_data(new_id));
        }

        static InstructionPacket<protocol_t> set_baudrate(typename protocol_t::id_t id, typename ct_t::baudrate_t baudrate, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::baudrate, protocol_t::pack_data(baudrate));
            else
                return write_t(id, ct_t::baudrate, protocol_t::pack_data(baudrate));
        }

        static InstructionPacket<protocol_t> set_torque_enabled(typename protocol_t::id_t id, bool enabled, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::torque_enable, protocol_t::pack_data((typename ct_t::torque_enable_t)(enabled ? 1 : 0)));
            else
                return write_t(id, ct_t::torque_enable, protocol_t::pack_data((typename ct_t::torque_enable_t)(enabled ? 1 : 0)));
        }

        static InstructionPacket<protocol_t> set_led(typename protocol_t::id_t id, bool enabled, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::led, protocol_t::pack_data(enabled ? ct_t::led_on : ct_t::led_off));
            else
                return write_t(id, ct_t::led, protocol_t::pack_data(enabled ? ct_t::led_on : ct_t::led_off));
        }

        static InstructionPacket<protocol_t> set_position(typename protocol_t::id_t id, typename ct_t::goal_pos_t pos, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::goal_pos, protocol_t::pack_data(pos));
            else
                return write_t(id, ct_t::goal_pos, protocol_t::pack_data(pos));
        }

        template <typename Id, typename Pos>
        static InstructionPacket<protocol_t> set_positions(const std::vector<Id>& ids, const std::vector<Pos>& pos)
        {
            assert(ids.size() == pos.size());
            std::vector<std::vector<uint8_t> > packed(pos.size());
            for (size_t i = 0; i < pos.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::goal_pos_t)pos[i]);

            return sync_write_t(ct_t::goal_pos, _get_typed<typename protocol_t::id_t>(ids), packed);
        }

        static InstructionPacket<protocol_t> set_speed(typename protocol_t::id_t id, typename ct_t::moving_speed_t speed, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::moving_speed, protocol_t::pack_data(speed));
            else
                return write_t(id, ct_t::moving_speed, protocol_t::pack_data(speed));
        }

        template <typename Id, typename Speed>
        static InstructionPacket<protocol_t> set_speeds(const std::vector<Id>& ids, const std::vector<Speed>& speeds)
        {
            assert(ids.size() == speeds.size());
            std::vector<std::vector<uint8_t> > packed(speeds.size());
            for (size_t i = 0; i < speeds.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::moving_speed_t)speeds[i]);

            return sync_write_t(ct_t::moving_speed, _get_typed<typename protocol_t::id_t>(ids), packed);
        }

        static InstructionPacket<protocol_t> set_torque_limit(typename protocol_t::id_t id, typename ct_t::torque_limit_t torque_limit, bool reg = false)
        {
            if (reg)
                return reg_write_t(id, ct_t::torque_limit, protocol_t::pack_data(torque_limit));
            else
                return write_t(id, ct_t::torque_limit, protocol_t::pack_data(torque_limit));
        }

        template <typename Id, typename TorqueLimit>
        static InstructionPacket<protocol_t> set_torque_limits(const std::vector<Id>& ids, const std::vector<TorqueLimit>& torque_limits)
        {
            assert(ids.size() == torque_limits.size());
            std::vector<std::vector<uint8_t> > packed(torque_limits.size());
            for (size_t i = 0; i < torque_limits.size(); i++)
                packed[i] = protocol_t::pack_data((typename ct_t::torque_limit_t)torque_limits[i]);

            return sync_write_t(ct_t::torque_limit, _get_typed<typename protocol_t::id_t>(ids), packed);
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
    };
}
}

#endif
