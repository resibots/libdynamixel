#ifndef DYNAMIXEL_BAD_PACKET_ERROR_HPP_
#define DYNAMIXEL_BAD_PACKET_ERROR_HPP_

#include <string>
#include <stdint.h>

#include "error.hpp"

namespace dynamixel {
    namespace errors {

        class BadPacket : public Error {
        public:
            BadPacket(const std::vector<uint8_t> packet, const char* msg) : _packet(packet)
            {
                this->_msg = msg;
            }

            const std::vector<uint8_t> packet() const
            {
                return _packet;
            }

            virtual std::ostream& print(std::ostream& os) const override
            {
                // output the error message
                os << _msg;

                // add the packet itself
                os << " ( " << std::hex;
                for (uint8_t byte : _packet) {
                    os << std::setfill('0') << std::setw(2) << (int)byte << " ";
                }
                os << ")" << std::dec;

                return os;
            }

        private:
            const std::vector<uint8_t> _packet;
        };
    }
}

#endif
