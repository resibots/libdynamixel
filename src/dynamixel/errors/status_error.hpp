#ifndef DYNAMIXEL_STATUS_ERROR_HPP_
#define DYNAMIXEL_STATUS_ERROR_HPP_

#include <string>
#include <stdint.h>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        /** @ingroup errors
        **/
        class StatusError : public Error {
        public:
            StatusError(uint8_t id, uint8_t protocol, uint8_t error_byte, const char* msg) : _id(id), _protocol(protocol), _error_byte(error_byte)
            {
                this->_msg = msg;
            }

            StatusError(uint8_t id, uint8_t protocol, uint8_t error_byte, const std::string& msg) : _id(id), _protocol(protocol), _error_byte(error_byte)
            {
                this->_msg = msg;
            }

            uint8_t id() const
            {
                return _id;
            }

            uint8_t protocol() const
            {
                return _protocol;
            }

            uint8_t error_byte() const
            {
                return _error_byte;
            }

        private:
            uint8_t _id, _protocol, _error_byte;
        };
    }
}

#endif
