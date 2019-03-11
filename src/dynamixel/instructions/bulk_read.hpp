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
            BulkRead(const std::vector<typename T::address_t>& address, const std::vector<typename T::id_t>& ids,
                const std::vector<uint8_t>& data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::bulk_read, _get_parameters(address, ids, data_length)) {}
            // const std::vector<uint8_t>& address
        protected:
            std::vector<uint8_t> _get_parameters(const std::vector<uint8_t>& address, const std::vector<typename T::id_t>& ids,
                const std::vector<uint8_t>& data_length)
            {
                if (ids.size() == 0)
                    throw errors::Error("BulkRead: ids vector of size zero");

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

            std::vector<uint8_t> _get_parameters(const std::vector<uint16_t>& address, const std::vector<typename T::id_t>& ids,
                const std::vector<uint16_t>& data_length)
            {
                if (ids.size() == 0)
                    throw errors::Error("BulkRead: ids vector of size zero");

                std::vector<uint8_t> parameters(3 * ids.size() + 3);

                parameters[0] = 0x00;
                for (size_t m = 0; m < ids.size(); m++) {
                    parameters[m + 1] = (uint8_t)(data_length[m] & 0xFF);
                    parameters[m + 2] = (uint8_t)(data_length[m] >> 8) & 0xFF;
                    parameters[m + 3] = (uint8_t)(ids[m] & 0xFF);
                    parameters[m + 4] = (uint8_t)(ids[m] >> 8) & 0xFF;
                    parameters[m + 5] = (uint8_t)(address[m] & 0xFF);
                    parameters[m + 6] = (uint8_t)(address[m] >> 8) & 0xFF;
                }

                return parameters;
            }
        };
    } // namespace instructions
} // namespace dynamixel

#endif
