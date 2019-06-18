#ifndef DYNAMIXEL_ERRORS_UNPACK_ERROR_HPP_
#define DYNAMIXEL_ERRORS_UNPACK_ERROR_HPP_

#include <sstream>
#include <stdint.h>
#include <string>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        class UnpackError : public Error {
        public:
            using size_type = std::vector<uint8_t>::size_type;
            UnpackError(uint8_t protocol, size_type size, size_type expected_size)
                : _protocol(protocol), _size(size), _expected_size(expected_size)
            {
                std::stringstream err_message;
                err_message << "Unpack: error while unpacking data in protocol "
                            << (int)_protocol << "; expected "
                            << (int)_expected_size << " bytes and got "
                            << (int)_size << " bytes.";
                this->_msg = err_message.str();
            }

            uint8_t protocol() const
            {
                return _protocol;
            }

            size_type size() const
            {
                return _size;
            }

            size_type expected_size() const
            {
                return _expected_size;
            }

        private:
            uint8_t _protocol;
            size_type _size, _expected_size;
        };
    } // namespace errors
} // namespace dynamixel

#endif
