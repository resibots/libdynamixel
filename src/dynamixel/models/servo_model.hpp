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

#define BASE_READ_FIELD(Name) \
    static InstructionPacket<protocol_t> get_##Name(typename protocol_t::id_t id)   \
    {                                                                                                                                     \
        return read_t(id, ct_t::Name, sizeof(typename ct_t::Name##_t));                           \
    }

#define BASE_WRITE_FIELD(Name) \
    static InstructionPacket<protocol_t> set_##Name(typename protocol_t::id_t id, typename ct_t::Name##_t value, bool reg = false)  \
    {                                                                                                                                                                                                                           \
            if (reg)                                                                                                                                                                                                          \
                return reg_write_t(id, ct_t::Name, protocol_t::pack_data(value));                                                                                                \
            else                                                                                                                                                                                                               \
                return write_t(id, ct_t::Name, protocol_t::pack_data(value));                                                                                                       \
    }

#define BASE_READ_WRITE_FIELD(Name) BASE_READ_FIELD(Name) \
    BASE_WRITE_FIELD(Name)

#define READ_FIELD(Name) \
    static InstructionPacket<typename ServoModel<Model>::protocol_t> get_##Name(typename ServoModel<Model>::protocol_t::id_t id)   \
    {                                                                                                                                     \
        return typename ServoModel<Model>::read_t(id, ServoModel<Model>::ct_t::Name, sizeof(typename ServoModel<Model>::ct_t::Name##_t));                           \
    }

#define WRITE_FIELD(Name) \
    static InstructionPacket<typename ServoModel<Model>::protocol_t> set_##Name(typename ServoModel<Model>::protocol_t::id_t id, typename ServoModel<Model>::ct_t::Name##_t value, bool reg = false)  \
    {                                                                                                                                                                                                                           \
            if (reg)                                                                                                                                                                                                          \
                return typename ServoModel<Model>::reg_write_t(id, ServoModel<Model>::ct_t::Name, ServoModel<Model>::protocol_t::pack_data(value));                                                                                                \
            else                                                                                                                                                                                                               \
                return typename ServoModel<Model>::write_t(id, ServoModel<Model>::ct_t::Name, ServoModel<Model>::protocol_t::pack_data(value));                                                                                                       \
    }

#define WRITE_BOOL_FIELD(Name, Bool_Name) \
    static InstructionPacket<typename ServoModel<Model>::protocol_t> set_##Name(typename ServoModel<Model>::protocol_t::id_t id, bool value, bool reg = false)  \
    {                                                                                                                                                                                                                           \
            if (reg)                                                                                                                                                                                                          \
                return typename ServoModel<Model>::reg_write_t(id, ServoModel<Model>::ct_t::Name, ServoModel<Model>::protocol_t::pack_data(value ? ServoModel<Model>::ct_t::Bool_Name##_on : ServoModel<Model>::ct_t::Bool_Name##_off)); \
            else                                                                                                                                                                                                               \
                return typename ServoModel<Model>::write_t(id, ServoModel<Model>::ct_t::Name, ServoModel<Model>::protocol_t::pack_data(value ? ServoModel<Model>::ct_t::Bool_Name##_on : ServoModel<Model>::ct_t::Bool_Name##_off));                                                                                                       \
    }

#define READ_WRITE_FIELD(Name) READ_FIELD(Name) \
    WRITE_FIELD(Name)

namespace dynamixel {
namespace models {
    template <class Model>
    struct ServoModel {
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

        BASE_READ_FIELD(model_number);
        BASE_READ_FIELD(firmware_version);
        BASE_READ_WRITE_FIELD(id);
        BASE_READ_WRITE_FIELD(baudrate);
        BASE_READ_WRITE_FIELD(return_delay_time);
        BASE_READ_WRITE_FIELD(highest_temperature_limit);
        BASE_READ_WRITE_FIELD(highest_voltage_limit);
        BASE_READ_WRITE_FIELD(lowest_voltage_limit);
        BASE_READ_WRITE_FIELD(status_return_level);
        BASE_READ_WRITE_FIELD(alarm_shutdown);
        BASE_READ_WRITE_FIELD(torque_enable);
        BASE_READ_WRITE_FIELD(goal_position);
        BASE_READ_WRITE_FIELD(moving_speed);
        BASE_READ_FIELD(present_position);
        BASE_READ_FIELD(present_speed);
        BASE_READ_FIELD(present_voltage);
        BASE_READ_FIELD(present_temperature);
        BASE_READ_FIELD(registered);
        BASE_READ_FIELD(moving);

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

        template <typename T, typename U>
        static std::vector<T> get_typed(std::vector<U> vector)
        {
            std::vector<T> result(vector.size());
            for (size_t i = 0; i < vector.size(); i++)
                result[i] = (T)vector[i];

            return result;
        }
    };

    template <class Model>
    struct AngleLimitServoModel {
        READ_WRITE_FIELD(cw_angle_limit);
        READ_WRITE_FIELD(ccw_angle_limit);
    };

    template <class Model>
    struct DriveModeServoModel {
        READ_WRITE_FIELD(drive_mode);
    };

    template <class Model>
    struct MaxTorqueServoModel {
        READ_WRITE_FIELD(max_torque);
    };

    template <class Model>
    struct AlarmLedServoModel {
        READ_WRITE_FIELD(alarm_led);
    };

    template <class Model>
    struct MultiTurnOffsetServoModel {
        READ_WRITE_FIELD(multi_turn_offset);
    };

    template <class Model>
    struct ResolutionDividerServoModel {
        READ_WRITE_FIELD(resolution_divider);
    };

    template <class Model>
    struct SingleLedServoModel {
        READ_FIELD(led);
        WRITE_BOOL_FIELD(led, led);
    };

    template <class Model>
    struct RGBLedServoModel {
        READ_FIELD(led_r);
        READ_FIELD(led_g);
        READ_FIELD(led_b);
        WRITE_BOOL_FIELD(led_r, led);
        WRITE_BOOL_FIELD(led_g, led);
        WRITE_BOOL_FIELD(led_b, led);
    };

    template <class Model>
    struct DynRGBLedServoModel {
        READ_WRITE_FIELD(led_r);
        READ_WRITE_FIELD(led_g);
        READ_WRITE_FIELD(led_b);
    };

    template <class Model>
    struct ComplianceServoModel {
        READ_WRITE_FIELD(cw_compliance_margin);
        READ_WRITE_FIELD(ccw_compliance_margin);
        READ_WRITE_FIELD(cw_compliance_slope);
        READ_WRITE_FIELD(ccw_compliance_slope);
    };

    template <class Model>
    struct PidServoModel {
        READ_WRITE_FIELD(d_gain);
        READ_WRITE_FIELD(i_gain);
        READ_WRITE_FIELD(p_gain);
    };

    template <class Model>
    struct TorqueLimitServoModel {
        READ_WRITE_FIELD(torque_limit);

        template <typename Id, typename TorqueLimit>
        static InstructionPacket<typename ServoModel<Model>::protocol_t> set_torque_limits(const std::vector<Id>& ids, const std::vector<TorqueLimit>& torque_limits)
        {
            assert(ids.size() == torque_limits.size());
            std::vector<std::vector<uint8_t> > packed(torque_limits.size());
            for (size_t i = 0; i < torque_limits.size(); i++)
                packed[i] = ServoModel<Model>::protocol_t::pack_data((typename ServoModel<Model>::ct_t::torque_limit_t)torque_limits[i]);

            return typename ServoModel<Model>::sync_write_t(ServoModel<Model>::ct_t::torque_limit, ServoModel<Model>::template get_typed<typename ServoModel<Model>::protocol_t::id_t>(ids), packed);
        }
    };

    template <class Model>
    struct PresentLoadServoModel {
        READ_FIELD(present_load);
    };

    template <class Model>
    struct LockServoModel {
        READ_FIELD(lock);
        WRITE_BOOL_FIELD(lock, lock);
    };

    template <class Model>
    struct PunchServoModel {
        READ_WRITE_FIELD(punch);
    };

    template <class Model>
    struct SensedCurrentServoModel {
        READ_FIELD(sensed_current);
    };

    template <class Model>
    struct CurrentServoModel {
        READ_FIELD(current);
    };

    template <class Model>
    struct TorqueControlModeServoModel {
        READ_FIELD(torque_control_mode_enabled);
        WRITE_BOOL_FIELD(torque_control_mode_enabled, torque_control_mode_enabled);
    };

    template <class Model>
    struct GoalTorqueServoModel {
        READ_WRITE_FIELD(goal_torque);
    };

    template <class Model>
    struct GoalAccelerationServoModel {
        READ_WRITE_FIELD(goal_acceleration);
    };
}
}

#endif
