#ifndef DYNAMIXEL_ERRORS_SERVO_LIMIT_ERROR_HPP_
#define DYNAMIXEL_ERRORS_SERVO_LIMIT_ERROR_HPP_

#include <sstream>
#include <stdint.h>
#include <string>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        class ServoLimitError : public Error {
        public:
            template <typename A, typename B, typename C, typename D>
            ServoLimitError(A id, B min, C max, D value, std::string type = "position (rad)") : _id(id), _max(max), _min(min), _value(value)
            {
                std::stringstream err_message;
                err_message << "Servo: error while setting " << type
                            << " in servo with ID " << (long long int)_id << ": \n"
                            << "Minimum: " << _min << ", Maximum: " << _max
                            << ", Given: " << _value;
                this->_msg = err_message.str();
            }

            int id() const
            {
                return _id;
            }

            double value() const
            {
                return _value;
            }

            double max() const
            {
                return _max;
            }

            double min() const
            {
                return _min;
            }

        private:
            int _id;
            double _max, _min, _value;
        };
    } // namespace errors
} // namespace dynamixel

#endif
