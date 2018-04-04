#ifndef DYNAMIXEL_INSTRUCTIONS_BULKREAD_HPP_
#define DYNAMIXEL_INSTRUCTIONS_BULKREAD_HPP_

#include <stdint.h>

#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        template <class T>
        class BulkRead : public InstructionPacket<T> {
        public:
            BulkRead(const std::vector<typename T::id_t>& ids,
                const std::vector<typename T::address_t> addresses,
                const std::vector<typename T::length_t> lengths)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read,
                      _get_parameters(ids, addresses, lengths)) {}

            // Version where the same field is retrieved from all servos
            BulkRead(const std::vector<typename T::id_t>& ids,
                typename T::address_t address,
                typename T::length_t length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read,
                      _get_parameters(ids, address, length)) {}

        protected:
            std::vector<uint8_t> _get_parameters(std::vector<uint8_t> ids,
                std::vector<uint8_t> addresses, std::vector<uint8_t> lengths)
            {
                _check_arguments(ids, addresses, lengths);

                std::vector<uint8_t> parameters(1 + 3 * ids.size());
                parameters[0] = 0;
                for (size_t i = 0; i < ids.size(); ++i) {
                    parameters[3 * i + 1] = lengths[i];
                    parameters[3 * i + 2] = ids[i];
                    parameters[3 * i + 3] = addresses[i];
                }
                return parameters;
            }

            std::vector<uint8_t> _get_parameters(std::vector<uint8_t> ids,
                std::vector<uint16_t> addresses, std::vector<uint16_t> lengths)
            {
                _check_arguments(ids, addresses, lengths);

                std::vector<uint8_t> parameters(1 + 5 * ids.size());
                parameters[0] = 0;
                for (size_t i = 0; i < ids.size(); ++i) {
                    parameters[5 * i + 1] = (uint8_t)(lengths[i] & 0xFF);
                    parameters[5 * i + 2] = (uint8_t)((lengths[i] >> 8) & 0xFF);
                    parameters[5 * i + 3] = ids[i];
                    parameters[5 * i + 4] = (uint8_t)(addresses[i] & 0xFF);
                    parameters[5 * i + 5] = (uint8_t)((addresses[i] >> 8) & 0xFF);
                }
                return parameters;
            }

            std::vector<uint8_t> _get_parameters(std::vector<uint8_t> ids,
                uint8_t address, uint8_t length)
            {
                if (ids.size() == 0)
                    throw errors::Error("SyncWrite: ids vector of size zero");

                std::vector<uint8_t> parameters(1 + 3 * ids.size());
                parameters[0] = 0;
                for (size_t i = 0; i < ids.size(); ++i) {
                    parameters[3 * i + 1] = length;
                    parameters[3 * i + 2] = ids[i];
                    parameters[3 * i + 3] = address;
                }
                return parameters;
            }

            std::vector<uint8_t> _get_parameters(std::vector<uint8_t> ids,
                uint16_t address, uint16_t length)
            {
                if (ids.size() == 0)
                    throw errors::Error("SyncWrite: ids vector of size zero");

                std::vector<uint8_t> parameters(1 + 5 * ids.size());
                parameters[0] = 0;
                for (size_t i = 0; i < ids.size(); ++i) {
                    parameters[5 * i + 1] = (uint8_t)(length & 0xFF);
                    parameters[5 * i + 2] = (uint8_t)((length >> 8) & 0xFF);
                    parameters[5 * i + 3] = ids[i];
                    parameters[5 * i + 4] = (uint8_t)(address & 0xFF);
                    parameters[5 * i + 5] = (uint8_t)((address >> 8) & 0xFF);
                }
                return parameters;
            }

            template <typename A, typename B, typename C>
            void _check_arguments(std::vector<A> ids, std::vector<B> addresses,
                std::vector<C> lengths)
            {
                if (ids.size() == 0)
                    throw errors::Error("SyncWrite: ids vector of size zero");
                if (ids.size() != addresses.size())
                    throw errors::Error("BulkRead: mismatching size for ids and addresses");
                if (ids.size() != lengths.size())
                    throw errors::Error("BulkRead: mismatching size for ids and lengths/sizes");
            }
        };
    } // namespace instructions
} // namespace dynamixel

#endif
