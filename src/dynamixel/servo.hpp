#ifndef DYNAMIXEL_SERVO_HPP_
#define DYNAMIXEL_SERVO_HPP_

#include <stdint.h>

namespace dynamixel {
    template <typename Controller>
    class BaseServo {
        Servo(const Controller& ctrl, uint32_t id, double recv_timeout) : _ctrl(ctrl), _id(id), _recv_timeout(recv_timeout) {}

        virtual bool ping() const = 0;

        virtual bool set_position() const = 0;

    protected:
        template <typename Protocol>
        bool send_recv(const InstructionPacket<T>& packet, StatusPacket<T>& status)
        {
            _ctrl.send(packet);
            return _ctrl.recv(_recv_timeout, status);
        }

        const Controller& _ctrl;
        uint32_t _id;
        double recv_timeout;
    };
}

template <Model>
class Servo : public BaseServo
{
    bool ping()
    {
        _send_recv(Model::Ping_t(_id));
    }
}

#endif
