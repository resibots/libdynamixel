#ifndef DYNAMIXEL_PROTOCOLS_PROTOCOL1_HPP_
#define DYNAMIXEL_PROTOCOLS_PROTOCOL1_HPP_

#include <stdint.h>
#include <vector>
#include <cassert>
#include <sstream>

#include "../errors/crc_error.hpp"
#include "../errors/status_error.hpp"
#include "../errors/unpack_error.hpp"

namespace dynamixel {
    namespace protocols {
        class Protocol1 {
        public:
            typedef uint8_t id_t;
            typedef uint8_t instr_t;
            typedef uint8_t address_t;
            typedef uint8_t length_t;

            static const id_t broadcast_id = 0xFE;

            struct Instructions {
                static const instr_t ping = 0x01;
                static const instr_t read = 0x02;
                static const instr_t write = 0x03;
                static const instr_t reg_write = 0x4;
                static const instr_t action = 0x05;
                static const instr_t factory_reset = 0x06;
                static const instr_t sync_write = 0x83;
            };

            static std::vector<uint8_t> pack_instruction(id_t id, instr_t instr)
            {
                const size_t packet_size = 2 // header
                    + 1 // id
                    + 1 // length
                    + 1 // instruction
                    + 1; // checksum

                std::vector<uint8_t> packet(packet_size);

                packet[0] = 0xFF;
                packet[1] = 0xFF;
                packet[2] = id;
                packet[3] = 2;
                packet[4] = instr;
                packet[5] = _checksum(packet);

                return packet;
            }

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

            static std::vector<uint8_t> pack_data(uint32_t data)
            {
                throw errors::Error("pack_data for unsigned int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static std::vector<uint8_t> pack_data(int32_t data)
            {
                throw errors::Error("pack_data for int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static void unpack_data(const std::vector<uint8_t>& packet, uint8_t& res)
            {
                if (packet.size() == 0)
                    throw errors::UnpackError(1);
                res = packet[0];
            }

            static void unpack_data(const std::vector<uint8_t>& packet, uint16_t& res)
            {
                if (packet.size() != 2)
                    throw errors::UnpackError(1);
                res = (((uint16_t)packet[1]) << 8) | ((uint16_t)packet[0]);
            }

            static void unpack_data(const std::vector<uint8_t>& packet, uint32_t& res)
            {
                throw errors::Error("unpack_data for unsigned int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static void unpack_data(const std::vector<uint8_t>& packet, int32_t& res)
            {
                throw errors::Error("unpack_data for int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static bool detect_status_header(const std::vector<uint8_t>& packet)
            {
                if (packet.size() >= 1 && packet[0] != 0xFF)
                    return false;
                if (packet.size() >= 2 && packet[1] != 0xFF)
                    return false;
                return true;
            }

            /** Decodes the content of a status packet recieved from the servos

            This method is only used by the StatusPacket class, to make it generic
            with regard to the protocol version.

            @param packet data of the recieved packet
            @param id id of the sending actuator
            @param parameters parameters of the status packet, filled by unpack_status

            @return true if and only if the status packet is valid (well formated)

            @see unpack_status in protocol2.hpp
        **/
            static bool unpack_status(const std::vector<uint8_t>& packet, id_t& id, std::vector<uint8_t>& parameters)
            {
                // 6 is the size of the smallest packets (no params)
                if (packet.size() < 6)
                    return false;

                if (packet[0] != 0xFF || packet[1] != 0xFF)
                    throw errors::Error("Status: bad packet header");

                id = packet[2];

                // Check that the actual length of the packet equals the one written
                // in the packet itself
                length_t length = packet[3];
                if (packet[3] != packet.size() - 4)
                    return false;

                uint8_t error = packet[4];

                parameters.clear();
                for (size_t i = 0; i < length - 2; ++i)
                    parameters.push_back(packet[5 + i]);

                // Compute checksum and compare with the one we recieved
                uint8_t checksum = _checksum(packet);

                if (checksum != packet.back())
                    throw errors::CrcError(id, 1, checksum, packet.back());

                if (error != 0) {
                    std::stringstream err_message;
                    err_message << ((int32_t)id) << ": ";
                    if (error & 1) // bit 0
                        err_message << "Input voltage error, ";
                    if (error & 2) // bit 1
                        err_message << "Angle limit error, ";
                    if (error & 4) // bit 2
                        err_message << "Overheating error, ";
                    if (error & 8) // bit 3
                        err_message << "Range error, ";
                    if (error & 16) // bit 4
                        err_message << "Checksum error, ";
                    if (error & 32) // bit 5
                        err_message << "Overload error, ";
                    if (error & 64) // bit 6
                        err_message << "Instruction error, ";

                    throw errors::StatusError(id, 1, error, "Status: error while decoding packet with ID " + err_message.str().substr(0, err_message.str().length() - 2));
                }

                return true;
            }

        protected:
            static uint8_t _checksum(const std::vector<uint8_t>& packet)
            {
                if (packet.size() == 0)
                    throw errors::Error("Checksum (protocol 1): cannot compute checksum, the packet is empty");
                int sum = 0;
                for (size_t i = 2; i < packet.size() - 1; ++i)
                    sum += packet[i];
                uint8_t checksum = (sum & 0xFF);
                if (!(sum > 255) && (sum != checksum))
                    throw errors::CrcError(packet[2], 1, checksum, sum);
                return ~checksum;
            }
        };
    }
}

#endif
