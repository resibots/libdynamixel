#include <iostream>
#include <boost/lexical_cast.hpp>
#include "status.hpp"

namespace dynamixel
{
    bool Status::decode_packet(const byte_t* packet, int packet_size)
    {
        // 5 is the size of the smallest packets (no params)
        if (packet_size < 5)
            return false;

        if (packet[0] != 0xFF || packet[1]  != 0xFF)
            throw Error("Status: bad packet header");

        _id = packet[2];
        _length = packet[3];

        if (_length != packet_size - 4)
        {
            _length = -1;
            _id = -1;
            return false;
        }

        byte_t error = packet[4];
	_params.clear();
        for (size_t i = 0; i < _length - 2; ++i)
            _params.push_back(packet[5 + i]);
        byte_t checksum = compute_checksum(packet, packet_size);

        if (checksum != packet[packet_size - 1])
            throw Error("Status: checksum error while decoding packet");

        if (error != 0)
        {
            std::string error_str = boost::lexical_cast<std::string>(_id);
            // we could have many errors in the same packet ?
            if (error & 1) // bit 0
                error_str += "Input voltage error;";
            if (error & 2) // bit 1
                error_str += "Angle limit error;";
            if (error & 4) // bit 2
                error_str += "Overheating error;";
            if (error & 8) // bit 3
                error_str += "Range error;";
            if (error & 16) // bit 4
                error_str += "Checksum error;";
            if (error & 32) // bit 5
            {
                error_str += "Overload error;";
                std::cout<<"Status: error while decoding packet: " + error_str<<std::endl;
                return true;
            }
            if (error & 64) // bit 6
                error_str += "Instruction error;";
            throw Error(std::string("Status: error while decoding packet: ") + error_str);
        }
        return true;
    }

    std::ostream& Status::print(std::ostream& os) const
    {
        if (_length == -1)
        {
            os<<"no packet or invalid";
            return os;
        }
        os<<"id="<<_id<<" length="<<_length<<" params=";
        for (size_t i = 0; i < _params.size(); ++i)
            os<<(int)_params[i]<<" ";
        os<<"["<<_params.size()<<"]";
        return os;
    }
}
