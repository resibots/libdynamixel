#ifndef DYNAMIXEL_PROTOCOLS_PROTOCOL1_HPP_
#define DYNAMIXEL_PROTOCOLS_PROTOCOL1_HPP_

#include <cassert>
#include <sstream>
#include <stdint.h>
#include <vector>

#include "../errors/bad_packet.hpp"
#include "../errors/crc_error.hpp"
#include "../errors/status_error.hpp"
#include "../errors/unpack_error.hpp"

namespace dynamixel {
    namespace protocols {
        class Protocol1 {
        public:
            // version of the protocol
            static constexpr uint8_t version = 1;

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
                // available only for the MX series
                static const instr_t bulk_read = 0x92;
            };

            enum DecodeState {
                INVALID,
                ONGOING,
                DONE
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

            static std::vector<std::vector<uint8_t>> pack_data(const std::vector<uint8_t>& data)
            {
                if (data.size() == 0)
                    throw errors::Error("pack_data was given an empty "
                                        "data vector");

                std::vector<std::vector<uint8_t>> packed(data.size());
                // std::vector<std::vector<uint8_t>> packed(data.size());
                for (auto datum : data) {
                    packed.push_back(pack_data(datum));
                }
                return packed;
            }

            static std::vector<std::vector<uint8_t>> pack_data(const std::vector<uint16_t>& data)
            {
                if (data.size() == 0)
                    throw errors::Error("pack_data was given an empty "
                                        "data vector");

                std::vector<std::vector<uint8_t>> packed(data.size());
                for (auto datum : data) {
                    packed.push_back(pack_data(datum));
                }
                return packed;
                // std::vector<uint8_t> packed(2 * data.size());
                // for (auto datum : data) {
                //     packed.push_back((uint8_t)(datum & 0xFF));
                //     packed.push_back((uint8_t)((datum >> 8) & 0xFF));
                // }
                // return packed;
            }

            static std::vector<std::vector<uint8_t>> pack_data(const std::vector<uint32_t>& data)
            {
                throw errors::Error("pack_data for unsigned int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static std::vector<std::vector<uint8_t>> pack_data(const std::vector<int32_t>& data)
            {
                throw errors::Error("pack_data for int (32 bits) not "
                                    "implemented in Protocol1");
            }

            static void unpack_data(const std::vector<uint8_t>& packet, uint8_t& res)
            {
                if (packet.size() != 1)
                    throw errors::UnpackError(1, packet.size(), 1);
                res = packet[0];
            }

            static void unpack_data(const std::vector<uint8_t>& packet, uint16_t& res)
            {
                if (packet.size() != 2)
                    throw errors::UnpackError(1, packet.size(), 2);
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

            /** Decodes the content of a status packet received from the servos

            This method is only used by the StatusPacket class, to make it
            independent of the protocol version.

            @param packet content of the received packet
            @param id id of the sending actuator
            @param parameters parameters of the status packet, filled by unpack_status
            @param throw_exceptions boolean telling to throw exceptions if the
                packet is malformed

            @return the state of the packet unpacking

            @see unpack_status in protocol2.hpp
            **/
            static DecodeState
            unpack_status(const std::vector<uint8_t>& packet, id_t& id,
                std::vector<uint8_t>& parameters, length_t& length,
                bool throw_exceptions = false)
            {
                if (!detect_status_header(packet)) {
                    if (throw_exceptions)
                        throw errors::BadPacket(packet, "Bad packet header");
                    return INVALID;
                }

                // 6 is the size of the smallest packets (no params)
                if (packet.size() < 6)
                    return ONGOING;

                id = packet[2];

                // Check that the actual length of the packet equals the one written
                // in the packet itself
                // ---------------------------------------------------------
                // FIXME: oroginal version is `length_t length = packet[3];`
                // ---------------------------------------------------------
                length = packet[3];
                if (length < 2) {
                    if (throw_exceptions) {
                        std::stringstream message;
                        message << "Declared packet length (";
                        message << (int32_t)length << ") is too small.";
                        throw errors::BadPacket(packet, message.str().c_str());
                    }
                    return INVALID;
                }
                if (length > packet.size() - 4)
                    return ONGOING;

                // Compute checksum and compare with the one we received
                uint8_t checksum = _checksum(packet);
                if (checksum != packet.back()) {
                    if (throw_exceptions)
                        throw errors::CrcError(id, 1, checksum, packet.back());
                    return INVALID;
                }

                uint8_t error = packet[4];

                if (error != 0) {
                    std::stringstream err_message;
                    err_message << "Actuator with ID " << ((int32_t)id)
                                << " reported the following error(s): ";
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

                    throw errors::StatusError(id, 1, error,
                        err_message.str().substr(0, err_message.str().length() - 2));
                }

                parameters.clear();
                for (size_t i = 0; i < length - 2; ++i)
                    parameters.push_back(packet[5 + i]);

                return DONE;
            }

        protected:
            /** Check if the packet contains a header.

                @param packet data of the received packet

                @return true if and only if a full header was found at the
                    beginning of the packet
            **/
            static bool detect_status_header(const std::vector<uint8_t>& packet)
            {
                if (packet.size() == 0)
                    return false;
                if (packet.size() >= 1 && packet[0] != 0xFF)
                    return false;
                if (packet.size() >= 2 && packet[1] != 0xFF)
                    return false;
                return true;
            }

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
    } // namespace protocols
} // namespace dynamixel

#endif
