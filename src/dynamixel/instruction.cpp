#include "instruction.hpp"

namespace dynamixel
{
    void Instruction :: _build_packet(byte_t id, 
            byte_t instruction, 
            const std::vector<byte_t>& params)
    {
        int old_packet_size = _packet_size;
        _packet_size = 
            2	// header
            + 1	// id
            + 1	// size
            + 1	// instruction
            + params.size() // params
            + 1;	// checksum
        if (old_packet_size != _packet_size)
        {
            if (_packet != 0x0)
                delete[] _packet;
            _packet = new byte_t[_packet_size];
        }
        _packet[0] = 0xFF;
        _packet[1] = 0xFF;
        _packet[2] = id;
        _packet[3] = (byte_t)(params.size() + 2);
        _packet[4] = instruction;
        for (size_t i = 0; i < params.size(); ++i)
            _packet[5 + i] = params[i];
        _packet[_packet_size - 1] = compute_checksum(_packet, _packet_size);    
    }

}
