#ifndef DYNAMIXEL_INSTRUCTIONS_BULK_READ_HPP_
#define DYNAMIXEL_INSTRUCTIONS_BULK_READ_HPP_

#include <cassert>
#include <stdint.h>

#include "../errors/error.hpp"
#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        template <class T>
        class BulkRead : public InstructionPacket<T> {
        public:
            BulkRead(const std::vector<typename T::id_t>& ids, const std::vector<typename T::address_t>& address,
                const std::vector<uint8_t>& data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read, _get_parameters(ids, address, data_length)) {}

            BulkRead(const std::vector<typename T::id_t>& ids, const typename T::address_t& address,
                const uint8_t& data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read, _get_parameters(ids, address, data_length)) {}

            BulkRead(const std::vector<typename T::id_t>& ids, const std::vector<typename T::address_t>& address,
                const std::vector<uint16_t>& data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read, _get_parameters(ids, address, data_length)) {}

            BulkRead(const std::vector<typename T::id_t>& ids, const typename T::address_t& address,
                const uint16_t& data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read, _get_parameters(ids, address, data_length)) {}

        protected:
            std::vector<uint8_t> _get_parameters(const std::vector<typename T::id_t>& ids, const std::vector<uint8_t>& address,
                const std::vector<uint8_t>& data_length)
            {
                _check_arguments(ids, address, data_length);

                std::vector<uint8_t> parameters(3 * ids.size() + 3);

                parameters[0] = 0x00;

                size_t curr = 1;

                for (size_t i = 0; i < ids.size(); i++) {

                    parameters[curr++] = data_length[i];
                    parameters[curr++] = ids[i];
                    parameters[curr++] = address[i];
                }

                return parameters;
            }

            std::vector<uint8_t> _get_parameters(const std::vector<typename T::id_t>& ids, const uint8_t& address,
                const uint8_t& data_length)
            {
                if (ids.size() == 0)
                    throw errors::Error("BulkRead: ids vector of size zero");

                std::vector<uint8_t> parameters(3 * ids.size() + 3);

                parameters[0] = 0x00;

                size_t curr = 1;

                for (size_t i = 0; i < ids.size(); i++) {

                    parameters[curr++] = data_length;
                    parameters[curr++] = ids[i];
                    parameters[curr++] = address;
                }

                return parameters;
            }

            std::vector<uint8_t> _get_parameters(const std::vector<typename T::id_t>& ids, const std::vector<uint16_t>& address,
                const std::vector<uint16_t>& data_length)
            {
                _check_arguments(ids, address, data_length);

                std::vector<uint8_t> parameters(5 * ids.size() + 5);
                size_t curr = 0;

                for (size_t m = 0; m < ids.size(); m++) {
                    parameters[curr++] = ids[m];
                    parameters[curr++] = (uint8_t)(address[m] & 0xFF);
                    parameters[curr++] = (uint8_t)(address[m] >> 8) & 0xFF;
                    parameters[curr++] = (uint8_t)(data_length[m] & 0xFF);
                    parameters[curr++] = (uint8_t)(data_length[m] >> 8) & 0xFF;
                }

                return parameters;
            }

            std::vector<uint8_t> _get_parameters(const std::vector<typename T::id_t>& ids, const uint16_t& address,
                const uint16_t& data_length)
            {
                if (ids.size() == 0)
                    throw errors::Error("BulkRead: ids vector of size zero");

                std::vector<uint8_t> parameters(5 * ids.size() + 5);
                size_t curr = 0;

                for (size_t m = 0; m < ids.size(); m++) {
                    parameters[curr++] = ids[m];
                    parameters[curr++] = (uint8_t)(address & 0xFF);
                    parameters[curr++] = (uint8_t)(address >> 8) & 0xFF;
                    parameters[curr++] = (uint8_t)(data_length & 0xFF);
                    parameters[curr++] = (uint8_t)(data_length >> 8) & 0xFF;
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
