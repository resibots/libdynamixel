#ifndef DYNAMIXEL_INSTRUCTIONS_BULK_WRITE_HPP_
#define DYNAMIXEL_INSTRUCTIONS_BULK_WRITE_HPP_

#include <cassert>
#include <stdint.h>

#include "../errors/vector_size_errors.hpp"
#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        template <class T>
        class BulkWrite : public InstructionPacket<T> {
        public:
            // Only for protocol 2
            BulkWrite(const std::vector<typename T::id_t>& ids, const typename T::address_t& address,
                const uint16_t& data_length, const std::vector<std::vector<uint8_t>>& data)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_write, _get_parameters(ids, address, data_length, data)) {}

        protected:
            std::vector<uint8_t> _get_parameters(const std::vector<typename T::id_t>& ids, const uint16_t& address,
                const uint16_t& data_length, const std::vector<std::vector<uint8_t>>& data)
            {
                if (ids.size() == 0)
                    throw errors::VectorEmptyError("BulkWrite", "ids");

                std::vector<uint8_t> parameters((5 + data_length) * ids.size() + 5);
                size_t curr = 0;

                for (size_t m = 0; m < ids.size(); m++) {
                    parameters[curr++] = ids[m];
                    parameters[curr++] = (uint8_t)(address & 0xFF);
                    parameters[curr++] = (uint8_t)(address >> 8) & 0xFF;
                    parameters[curr++] = (uint8_t)(data_length & 0xFF);
                    parameters[curr++] = (uint8_t)(data_length >> 8) & 0xFF;
                    if (data[m].size() != data_length)
                        throw errors::Error("BulkWrite: mismatch between declared"
                                            "data length and data vector size");
                    for (size_t i = 0; i < data[m].size(); ++i) {
                        parameters[curr++] = data[m][i];
                    }
                }

                return parameters;
            }
        };
    } // namespace instructions
} // namespace dynamixel

#endif
