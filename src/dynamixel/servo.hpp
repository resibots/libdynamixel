#ifndef DYNAMIXEL_SERVO_HPP_
#define DYNAMIXEL_SERVO_HPP_

#include <stdint.h>

#include "instruction_packet.hpp"
#include "ping.hpp"
#include "read.hpp"
#include "write.hpp"
#include "servo_traits.hpp"

namespace dynamixel {
    template <class Model>
    class Servo {
    public:
        typedef ServoTraits<Model> traits;
        typedef typename traits::protocol_t protocol_t;
        typedef typename traits::CT ct_t;
        typedef Ping<protocol_t> ping_t;
        typedef Read<protocol_t> read_t;
        typedef Write<protocol_t> write_t;

        static InstructionPacket<protocol_t> ping(typename protocol_t::id_t id)
        {
            return ping_t(id);
        }

        static InstructionPacket<protocol_t> set_position(typename protocol_t::id_t id, typename ct_t::goal_pos_t pos)
        {
            return write_t(id, ct_t::goal_pos, protocol_t::pack_data(pos));
        }

        static InstructionPacket<protocol_t> led_on(typename protocol_t::id_t id)
        {
            return write_t(id, ct_t::led, protocol_t::pack_data((typename ct_t::led_t)1));
        }

        static InstructionPacket<protocol_t> led_off(typename protocol_t::id_t id)
        {
            return write_t(id, ct_t::led, protocol_t::pack_data((typename ct_t::led_t)(0)));
        }
    };
}

#endif
