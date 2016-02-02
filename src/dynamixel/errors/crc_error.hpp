#ifndef DYNAMIXEL_ERRORS_CRC_ERROR_HPP_
#define DYNAMIXEL_ERRORS_CRC_ERROR_HPP_

#include <string>
#include <stdint.h>

#include "error.hpp"

namespace dynamixel {
namespace errors {
    class CrcError : public Error {
    public:
        CrcError(uint8_t id, uint8_t protocol, uint32_t expected, uint32_t received) : _id(id), _protocol(protocol), _expected(expected), _received(received)
        {
            this->_msg = "Status: checksum error while decoding packet with ID " + boost::lexical_cast<std::string>((int32_t)id) + ": expected " + boost::lexical_cast<std::string>(expected) + ", received " + boost::lexical_cast<std::string>(received);
        }

        uint8_t id() const
        {
            return _id;
        }

        uint8_t protocol() const
        {
            return _protocol;
        }

        uint32_t expected() const
        {
            return _expected;
        }

        uint32_t received() const
        {
            return _received;
        }

    private:
        uint8_t _id, _protocol;
        uint32_t _expected, _received;
    };
}
}

#endif
