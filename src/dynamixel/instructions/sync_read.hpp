#ifndef DYNAMIXEL_INSTRUCTIONS_SYNC_READ_HPP_
#define DYNAMIXEL_INSTRUCTIONS_SYNC_READ_HPP_

#include <cassert>
#include <stdint.h>

#include "../errors/error.hpp"
#include "../instruction_packet.hpp"

namespace dynamixel {
    namespace instructions {
        template <class T>
        class SyncRead : public InstructionPacket<T> {
        public:
            SyncRead(typename T::address_t address, const std::vector<typename T::id_t>& ids, uint16_t data_length)
                : InstructionPacket<T>(T::broadcast_id, T::Instructions::sync_read, _get_parameters(address, ids, data_length)) {}

        protected:
            std::vector<uint8_t> _get_parameters(uint16_t address, const std::vector<typename T::id_t>& ids,
                uint16_t data_length)
            {
                if (ids.size() == 0)
                    throw errors::Error("SyncRead: ids vector of size zero");

                std::vector<uint8_t> parameters(ids.size() + 9);

                parameters[0] = (uint8_t)(address[m] & 0xFF);
                parameters[1] = (uint8_t)(address[m] >> 8) & 0xFF;
                parameters[2] = (uint8_t)(data_length[m] & 0xFF);
                parameters[3] = (uint8_t)(data_length[m] >> 8) & 0xFF;

                size_t curr = 4;

                for (size_t i = 0; i < ids.size(); ++i) {
                    parameters[curr++] = ids[i];
                }

                return parameters;
            }
        };
    } // namespace instructions
} // namespace dynamixel

#endif
