#ifndef DYNAMIXEL_STATUS_PACKET_HPP_
#define DYNAMIXEL_STATUS_PACKET_HPP_

#include <vector>
#include <stdint.h>
#include <cassert>

#include "./errors/error.hpp"

namespace dynamixel {
    /**
      This class represents packets that are received on the bus.

      @see Protocol1: http://support.robotis.com/en/product/actuator/dynamixel/communication/dxl_packet.htm#Status_Packet
      @see Protocol2: http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm
    **/
    template <class Protocol>
    class StatusPacket {
    public:
        using DecodeState = typename Protocol::DecodeState;

        StatusPacket() : _valid(false) {}

        bool valid() const { return _valid; }

        typename Protocol::id_t id() const
        {
            if (!_valid)
                throw errors::Error("StatusPacket: should be valid before calling member function: `id`");
            return _id;
        }

        const std::vector<uint8_t>& parameters() const
        {
            if (!_valid)
                throw errors::Error("StatusPacket: should be valid before calling member function: `parameters`");
            return _parameters;
        }

        DecodeState decode_packet(const std::vector<uint8_t>& packet, bool report_bad_packet = false)
        {
            DecodeState state = Protocol::unpack_status(packet, _id, _parameters, report_bad_packet);

            if (state == DecodeState::DONE)
                _valid = true;

            return state;
        }

        std::ostream& print(std::ostream& os) const
        {
            if (!_valid) {
                os << "no packet or invalid";
                return os;
            }

            os << "id=" << (unsigned int)_id << " params=";

            for (size_t i = 0; i < _parameters.size(); ++i)
                os << (unsigned int)_parameters[i] << " ";
            os << "[" << _parameters.size() << "]";

            return os;
        }

    protected:
        bool _valid;
        typename Protocol::id_t _id;
        std::vector<uint8_t> _parameters;
    };

    template <typename Protocol>
    inline std::ostream& operator<<(std::ostream& os, const StatusPacket<Protocol>& st)
    {
        return st.print(os);
    }
}

#endif
