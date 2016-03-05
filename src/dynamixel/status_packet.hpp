#ifndef DYNAMIXEL_STATUS_PACKET_HPP_
#define DYNAMIXEL_STATUS_PACKET_HPP_

#include <vector>
#include <stdint.h>
#include <cassert>

#include "./errors/error.hpp"

namespace dynamixel {
    template <class Protocol>
    class StatusPacket {
    public:
        StatusPacket() : _valid(false) { }

        bool valid() const { return _valid; }

        typename Protocol::id_t id() const
        {
            if(!_valid)
                throw errors::Error("StatusPacket: should be valid before calling member function: `id`");
            return _id;
        }

        const std::vector<uint8_t>& parameters() const
        {
            if(!_valid)
                throw errors::Error("StatusPacket: should be valid before calling member function: `parameters`");
            return _parameters;
        }

        bool decode_packet(const std::vector<uint8_t>& packet)
        {
            _valid = Protocol::unpack_status(packet, _id, _parameters);
            return _valid;
        }

        std::ostream& print(std::ostream& os) const
        {
            if (!_valid)
            {
                os << "no packet or invalid";
                return os;
            }

            os << "id=" << (unsigned int)_id << " params=";

            for (size_t i = 0; i < _parameters.size(); ++i)
                os << (unsigned int)_parameters[i] << " ";
            os << "["<<_parameters.size()<<"]";

            return os;
        }

    protected:
        bool _valid;
        typename Protocol::id_t _id;
        std::vector<uint8_t> _parameters;
    };

    template <typename Protocol>
    inline std::ostream& operator << (std::ostream & os, const StatusPacket<Protocol> &st)
    {
        return st.print(os);
    }
}

#endif
