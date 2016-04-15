#ifndef DYNAMIXEL_ERRORS_SERVO_LIMIT_ERROR_HPP_
#define DYNAMIXEL_ERRORS_SERVO_LIMIT_ERROR_HPP_

#include <string>
#include <stdint.h>
#include <sstream>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        class ServoLimitError : public Error {
        public:
            ServoLimitError(uint8_t id, uint32_t min, uint32_t max, uint32_t value, std::string type = "position") : _id(id), _max(max), _min(min), _value(value)
            {
                std::stringstream err_message;
                err_message << "Servo: error while setting " << type
                            << " in servo with ID " << (int)_id << ": \n"
                            << "Minimum: " << _min << ", Maximum: " << _max
                            << ", Given: " << _value;
                this->_msg = err_message.str();
            }

            uint8_t id() const
            {
                return _id;
            }

            uint32_t value() const
            {
                return _value;
            }

            uint32_t max() const
            {
                return _max;
            }

            uint32_t min() const
            {
                return _min;
            }

        private:
            uint8_t _id;
            uint32_t _max, _min, _value;
        };
    }
}

#endif
