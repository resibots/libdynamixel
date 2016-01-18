#ifndef DYNAMIXEL_INSTRUCTIONS_SYNC_WRITE_HPP_
#define DYNAMIXEL_INSTRUCTIONS_SYNC_WRITE_HPP_

#include <stdint.h>
#include <cassert>

#include "../instruction_packet.hpp"

namespace dynamixel {
namespace instructions {
    template <class T>
    class SyncWrite : public InstructionPacket<T> {
    public:
        SyncWrite(typename T::address_t address, const std::vector<typename T::id_t>& ids,
            const std::vector<std::vector<uint8_t> >& data)
            : InstructionPacket<T>(T::broadcast_id, T::Instructions::sync_write, _get_parameters(address, ids, data)) {}

    protected:

        std::vector<uint8_t> _get_parameters(uint8_t address, const std::vector<typename T::id_t>& ids,
            const std::vector<std::vector<uint8_t> >& data)
        {
            assert(ids.size());
            assert(ids.size() == data.size());

            typename T::length_t data_length = data[0].size();
            std::vector<uint8_t> parameters((data_length + 1) * ids.size() + 2);

            parameters[0] = address;
            parameters[1] = data_length;
            
            size_t curr = 2;

            for (size_t i = 0; i < ids.size(); ++i) {
                assert(data[i].size() == data_length);
                parameters[curr++] = ids[i];

                for (size_t j = 0; j < data_length; ++j)
                    parameters[curr++] = data[i][j];
            }

            std::cout << curr << "/" << parameters.size() << std::endl;

            return parameters;
        }

        std::vector<uint8_t> _get_parameters(uint16_t address, const std::vector<typename T::id_t>& ids,
            const std::vector<std::vector<uint8_t> >& data)
        {
            assert(ids.size());
            assert(ids.size() == data.size());


            typename T::length_t data_length = data[0].size();
            std::vector<uint8_t> parameters((data_length + 1) * ids.size() + 4);

            parameters[0] = (uint8_t)(address & 0xFF);
            parameters[1] = (uint8_t)((address >> 8) & 0xFF);
            parameters[2] = (uint8_t)(data_length & 0xFF);
            parameters[3] = (uint8_t)((data_length >> 8) & 0xFF);

            size_t curr = 4;

            for (size_t i = 0; i < ids.size(); ++i) {
                assert(data[i].size() == data_length);
                parameters[curr++] = ids[i];

                for (size_t j = 0; j < data_length; ++j)
                    parameters[curr++] = data[i][j];
            }

            std::cout << curr << "/" << data_length << std::endl;

            return parameters;
        }
    };
}
}

#endif
