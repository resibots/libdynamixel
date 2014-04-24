#ifndef _DYNAMIXEL_INSTRUCTION_HPP_
#define _DYNAMIXEL_INSTRUCTION_HPP_

#include "packet.hpp"
namespace dynamixel
{

    class Instruction
    {
        public:
            Instruction(byte_t id,
                        byte_t instruction) :
                        _packet(0x0), 
                        _packet_size(-1), 
                        _id(id), 
                        _instruction(instruction)
            {
                _build_packet(id, instruction, std::vector<byte_t>());
            }

            Instruction(byte_t id,
                        byte_t instruction,
                        byte_t param) :
                        _packet(0x0), 
                        _packet_size(-1), 
                        _id(id), 
                        _instruction(instruction)
            {
                std::vector<byte_t> p; 
                p.push_back(param);
                _build_packet(id, instruction, p);
            }

            Instruction(byte_t id,
                        byte_t instruction,
                        byte_t p1,
                        byte_t p2) :
                        _packet(0x0), 
                        _packet_size(-1), 
                        _id(id), 
                        _instruction(instruction)
            {
                std::vector<byte_t> p; 
                p.push_back(p1); p.push_back(p2);
                _build_packet(id, instruction, p);      
            }

            Instruction(byte_t id, 
                        byte_t instruction,
                        byte_t p1,
                        byte_t p2,
                        byte_t p3) :
                        _packet(0x0), 
                        _packet_size(-1), 
                        _id(id), 
                        _instruction(instruction)
            {
                std::vector<byte_t> p; 
                p.push_back(p1); p.push_back(p2); p.push_back(p3);
                _build_packet(id, instruction, p);      
            }

            Instruction(byte_t id,
                        byte_t instruction, 
                        const std::vector<byte_t>& params) :
                        _packet(0x0), 
                        _packet_size(-1), 
                        _id(id), 
                        _instruction(instruction)
            {
                _build_packet(id, instruction, params);
            }

            ~Instruction() 
            { 
                assert(_packet); 
                delete[] _packet; 
            }    

            const byte_t* packet() const 
            { 
                assert(_packet);
                assert(_packet[0] == 0xFF);
                assert(_packet[1] == 0xFF);
                return _packet;
            }

            const byte_t id() const 
            {
                assert(_packet);
                return _packet[2];
            }

            const byte_t instruction() const 
            { 
                assert(_packet);
                return _packet[4]; 
            }

            const size_t packet_size() const
            {
                return _packet_size;
            }

            // rebuid the same instruction but with different params
            void rebuild_packet(const std::vector<byte_t>& params)
            {
                _build_packet(_id, _instruction, params);
            }

        protected:
            void _build_packet(byte_t id,
                    byte_t instruction, 
                    const std::vector<byte_t>& params);
            byte_t* _packet;
            int _packet_size;
            byte_t _id;
            byte_t _instruction;

    };

}
#endif
