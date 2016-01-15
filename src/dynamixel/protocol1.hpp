#ifndef DYNAMIXEL_PROTOCOL1_HPP_
#define DYNAMIXEL_PROTOCOL1_HPP_

#include <stdint.h>
#include <vector>
#include <cassert>

#include "error.hpp"

namespace dynamixel {
    class Protocol1 {
    public:
        typedef uint8_t id_t;
        typedef uint8_t instr_t;
        typedef uint8_t address_t;
        typedef uint8_t length_t;

        struct Instructions {
            static const instr_t ping = 0x01;
            static const instr_t read = 0x02;
            static const instr_t write = 0x03;
            static const instr_t reg_write = 0x4;
            static const instr_t action = 0x05;
            static const instr_t factory_reset = 0x06;
            static const instr_t sync_write = 0x83;
        };

        static std::vector<uint8_t> pack_instruction(id_t id, instr_t instr, const std::vector<uint8_t>& parameters)
        {
            size_t packet_size = 2 // header
                + 1 // id
                + 1 // length
                + 1 // instruction
                + parameters.size() // parameters
                + 1; // checksum

            std::vector<uint8_t> packet(packet_size);

            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = id;
            packet[3] = (uint8_t)(parameters.size() + 2);
            packet[4] = instr;

            for (size_t i = 0; i < parameters.size(); ++i)
                packet[5 + i] = parameters[i];

            packet[packet_size - 1] = _checksum(packet);

            return packet;
        }

        static std::vector<uint8_t> pack_data(uint8_t data)
        {
            std::vector<uint8_t> packed(1);
            packed[0] = data;
            return packed;
        }

        static std::vector<uint8_t> pack_data(uint16_t data)
        {
            std::vector<uint8_t> packed(2);
            packed[0] = (uint8_t)(data & 0xFF);
            packed[1] = (uint8_t)((data >> 8) & 0xFF);
            return packed;
        }

        static bool unpack_status(const std::vector<uint8_t>& packet, id_t& id, std::vector<uint8_t>& parameters)
        {
            // 6 is the size of the smallest packets (no params)
            if (packet.size() < 6)
                return false;

            if (packet[0] != 0xFF || packet[1]  != 0xFF)
                throw Error("Status: bad packet header");

            if (packet[3] != packet.size() - 4)                
                return false;

            id = packet[2];
            length_t length = packet[3];
            uint8_t error = packet[4];

            parameters.clear();
            for (size_t i = 0; i < length - 2; ++i)
                parameters.push_back(packet[5 + i]);
            uint8_t checksum = _checksum(packet);

            if (checksum != packet.back())
                throw Error("Status: checksum error while decoding packet");

            /*if (error != 0)
            {
                std::string error_str = boost::lexical_cast<std::string>(_id);
                // we could have many errors in the same packet ?
                if (error & 1) // bit 0
                    error_str += " Input voltage error;";
                if (error & 2) // bit 1
                    error_str += " Angle limit error;";
                if (error & 4) // bit 2
                    error_str += " Overheating error;";
                if (error & 8) // bit 3
                    error_str += " Range error;";
                if (error & 16) // bit 4
                    error_str += " Checksum error;";
                if (error & 32) // bit 5
                {
                    error_str += "Overload error;";
                    std::cout<<"Status: error while decoding packet: " + error_str<<std::endl;
                    return true;
                }
                if (error & 64) // bit 6
                    error_str += "Instruction error;";
                throw Error(std::string("Status: error while decoding packet: ") + error_str);
            }*/
            return true;
        }

    protected:
        static uint8_t _checksum(const std::vector<uint8_t>& packet)
        {
            assert(packet.size());
            int sum = 0;
            for (size_t i = 2; i < packet.size() - 1; ++i)
                sum += packet[i];
            uint8_t checksum = (sum & 0xFF);
            assert((sum > 255) || (sum == checksum));
            return ~checksum;
        }
    };
}

#endif
