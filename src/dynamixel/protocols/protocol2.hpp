#ifndef DYNAMIXEL_PROTOCOLS_PROTOCOL2_HPP_
#define DYNAMIXEL_PROTOCOLS_PROTOCOL2_HPP_

#include <stdint.h>
#include <vector>
#include <cassert>
#include <sstream>

#include "../errors/crc_error.hpp"
#include "../errors/status_error.hpp"

namespace dynamixel {
namespace protocols {
    class Protocol2 {
    public:
        typedef uint8_t id_t;
        typedef uint8_t instr_t;
        typedef uint16_t address_t;
        typedef uint16_t length_t;

        static const id_t broadcast_id = 0xFE;

        struct Instructions {
            static const instr_t ping = 0x01;
            static const instr_t read = 0x02;
            static const instr_t write = 0x03;
            static const instr_t reg_write = 0x4;
            static const instr_t action = 0x05;
            static const instr_t factory_reset = 0x06;
            static const instr_t reboot = 0x08;
            static const instr_t sync_read = 0x82;
            static const instr_t sync_write = 0x83;
            static const instr_t bulk_read = 0x92;
            static const instr_t bulk_write = 0x93;
        };

        static std::vector<uint8_t> pack_instruction(id_t id, instr_t instr)
        {
            const size_t packet_size = 3 // header
                + 1 // reserved
                + 1 // id
                + 2 // length
                + 1 // instruction
                + 2; // checksum

            std::vector<uint8_t> packet(packet_size);

            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0xFD;
            packet[3] = 0x00;
            packet[4] = id;
            packet[5] = 3;
            packet[6] = 0;
            packet[7] = instr;
            uint16_t checksum = _checksum(packet);
            packet[8] = (uint8_t)(checksum & 0xFF);
            packet[9] = (uint8_t)((checksum >> 8) & 0xFF);

            return packet;
        }

        static std::vector<uint8_t> pack_instruction(id_t id, instr_t instr, const std::vector<uint8_t>& parameters)
        {
            size_t packet_size = 3 // header
                + 1 // reserved
                + 1 // id
                + 2 // length
                + 1 // instruction
                + parameters.size() // parameters
                + 2; // checksum

            std::vector<uint8_t> packet(packet_size);

            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0xFD;
            packet[3] = 0x00;
            packet[4] = id;
            packet[5] = (uint8_t)((parameters.size() + 3) & 0xFF);
            packet[6] = (uint8_t)(((parameters.size() + 3) >> 8) & 0xFF);
            packet[7] = instr;
            for (size_t i = 0; i < parameters.size(); ++i)
                packet[8 + i] = parameters[i];
            uint16_t checksum = _checksum(packet);
            packet[packet_size - 2] = (uint8_t)(checksum & 0xFF);
            packet[packet_size - 1] = (uint8_t)((checksum >> 8) & 0xFF);

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

        static std::vector<uint8_t> pack_data(uint32_t data)
        {
            std::vector<uint8_t> packed(4);
            packed[0] = (uint8_t)(data & 0xFF);
            packed[1] = (uint8_t)((data >> 8) & 0xFF);
            packed[2] = (uint8_t)((data >> 16) & 0xFF);
            packed[3] = (uint8_t)((data >> 24) & 0xFF);
            return packed;
        }

        static std::vector<uint8_t> pack_data(int32_t data)
        {
            std::vector<uint8_t> packed(4);
            packed[0] = (uint8_t)(data & 0xFF);
            packed[1] = (uint8_t)((data >> 8) & 0xFF);
            packed[2] = (uint8_t)((data >> 16) & 0xFF);
            packed[3] = (uint8_t)((data >> 24) & 0xFF);
            return packed;
        }

        static void unpack_data(const std::vector<uint8_t>& packet, uint8_t& res)
        {
            assert(packet.size());
            res = packet[0];
        }

        static void unpack_data(const std::vector<uint8_t>& packet, uint16_t& res)
        {
            assert(packet.size() == 2);
            res = (((uint16_t)packet[1]) << 8) | ((uint16_t)packet[0]);
        }

        static void unpack_data(const std::vector<uint8_t>& packet, uint32_t& res)
        {
            assert(packet.size() == 4);
            res = (((uint32_t)packet[3]) << 24) | (((uint32_t)packet[2]) << 16) | (((uint32_t)packet[1]) << 8) | ((uint32_t)packet[0]);
        }

        static void unpack_data(const std::vector<uint8_t>& packet, int32_t& res)
        {
            assert(packet.size() == 4);
            res = (((int32_t)packet[3]) << 24) | (((int32_t)packet[2]) << 16) | (((int32_t)packet[1]) << 8) | ((int32_t)packet[0]);
        }

        static bool unpack_status(const std::vector<uint8_t>& packet, id_t& id, std::vector<uint8_t>& parameters)
        {
            // 11 is the size of the smallest packets (no params)
            if (packet.size() < 11)
                return false;

            if (packet[0] != 0xFF || packet[1]  != 0xFF || packet[2]  != 0xFD)
                throw errors::Error("Status: bad packet header");

            length_t length = (((uint16_t)packet[6]) << 8) | packet[5];

            if (length != packet.size() - 7)
                return false;

            id = packet[4];
            uint8_t error = packet[8];

            parameters.clear();
            for (size_t i = 0; i < length - 4; ++i)
                parameters.push_back(packet[9 + i]);
            uint16_t checksum = _checksum(packet);
            uint16_t recv_checksum = (((uint16_t)packet.back()) << 8) | packet[packet.size() - 2];

            if (checksum != recv_checksum)
                throw errors::CrcError(id, 2, checksum, recv_checksum);

            if (error != 0)
            {
                std::stringstream err_message;
                err_message << ((int32_t)id) << ": ";
                if (error & 0x80)
                    err_message << "Device alert. Check Hardware Error field from Control Table; ";

                switch(error & 0x7F)
                {
                case 1:
                    err_message << "Result fail";
                    break;
                case 2:
                    err_message << "Instruction error";
                    break;
                case 3:
                    err_message << "CRC error";
                    break;
                case 4:
                    err_message << "Data range error";
                    break;
                case 5:
                    err_message << "Data length error";
                    break;
                case 6:
                    err_message << "Data limit error";
                    break;
                case 7:
                    err_message << "Access error";
                    break;
                }

                throw errors::StatusError(id, 2, error, "Status: error while decoding packet with ID " + err_message.str());
            }

            return true;
        }

    protected:
        static uint16_t _checksum(const std::vector<uint8_t>& packet)
        {
            assert(packet.size());
            uint16_t crc_accum = 0;

            uint16_t crc_table[256] = {
                0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011, 0x8033,
                0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022, 0x8063, 0x0066,
                0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072, 0x0050, 0x8055, 0x805F,
                0x005A, 0x804B, 0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9,
                0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB,
                0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE,
                0x00B4, 0x80B1, 0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087,
                0x0082, 0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
                0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1, 0x01E0,
                0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6,
                0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145, 0x814F,
                0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176, 0x017C, 0x8179,
                0x0168, 0x816D, 0x8167, 0x0162, 0x8123, 0x0126, 0x012C, 0x8129, 0x0138,
                0x813D, 0x8137, 0x0132, 0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E,
                0x0104, 0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317,
                0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
                0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371, 0x8353,
                0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5,
                0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 0x83F3, 0x03F6, 0x03FC,
                0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2, 0x83A3, 0x03A6, 0x03AC, 0x83A9,
                0x03B8, 0x83BD, 0x83B7, 0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B,
                0x038E, 0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E,
                0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7,
                0x02A2, 0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
                0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 0x8243,
                0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252, 0x0270, 0x8275,
                0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261, 0x0220, 0x8225, 0x822F,
                0x022A, 0x823B, 0x023E, 0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219,
                0x0208, 0x820D, 0x8207, 0x0202};

            for (size_t j = 0; j < packet.size() - 2; j++) {
                uint16_t i = ((uint16_t)(crc_accum >> 8) ^ packet[j]) & 0xFF;
                crc_accum = (crc_accum << 8) ^ crc_table[i];
            }

            return crc_accum;
        }
    };
}
}

#endif
