#ifndef DYNAMIXEL_ERRORS_UNPACK_ERROR_HPP_
#define DYNAMIXEL_ERRORS_UNPACK_ERROR_HPP_

#include <string>
#include <stdint.h>
#include <sstream>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        /** @ingroup errors
        **/
        class UnpackError : public Error {
        public:
            UnpackError(uint8_t protocol) : _protocol(protocol)
            {
                std::stringstream err_message;
                err_message << "Unpack: error while unpacking data in protocol: "
                            << (int)_protocol;
                this->_msg = err_message.str();
            }

            uint8_t protocol() const
            {
                return _protocol;
            }

        private:
            uint8_t _protocol;
        };
    }
}

#endif
