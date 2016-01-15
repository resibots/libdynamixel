#ifndef DYNAMIXEL_WRITE_HPP_
#define DYNAMIXEL_WRITE_HPP_

#include <stdint.h>

namespace dynamixel {
    template <class T>
    class Write : public Packet<T> {
    public:
        Write(typename T::id_t id, typename T::address_t address,
            const std::vector<uint8_t>& data)
            : Packet<T>(id, _get_payload(address, data)) {}

    protected:
        std::vector<uint8_t> _get_payload(uint8_t address,
            const std::vector<uint8_t>& data)
        {
            std::vector<uint8_t> payload(2 + data.size());
            payload[0] = T::Instructions::write;
            payload[1] = address;

            for (size_t i = 0; i < data.size(); ++i)
                payload[2 + i] = data[i];

            return payload;
        }

        std::vector<uint8_t> _get_payload(uint16_t address,
            const std::vector<uint8_t>& data)
        {
            std::vector<uint8_t> payload(3 + data.size());
            payload[0] = T::Instructions::write;
            payload[1] = (uint8_t)(address & 0x00ff);
            payload[2] = (uint8_t)((address >> 8) & 0x00ff);

            for (size_t i = 0; i < data.size(); ++i)
                payload[3 + i] = data[i];

            return payload;
        }
    };
}

#endif
