#ifndef DYNAMIXEL_VECTOR_SIZE_ERROR_HPP_
#define DYNAMIXEL_VECTOR_SIZE_ERROR_HPP_

#include <string>

#include "error.hpp"

namespace dynamixel {
    namespace errors {
        class VectorEmptyError : public Error {
        public:
            VectorEmptyError(const std::string& method_name,
                const std::string& vector_name)
                : _method_name(method_name), _vector_name(vector_name)
            {
                this->_msg = "The vector " + vector_name + ", passed to "
                    + method_name + " cannot be empty (size 0)";
            }

            std::string vector_name() const
            {
                return _vector_name;
            }

            std::string method_name() const
            {
                return _method_name;
            }

        protected:
            const std::string _method_name, _vector_name;
        };

        class VectorSizesDifferError : public Error {
        public:
            VectorSizesDifferError(const std::string& method_name,
                const std::string& name1, const std::string& name2)
                : _method_name(method_name)
            {
                this->_msg = "The vectors " + name1 + " and " + name2
                    + ", passed to " + method_name + " should have the same size.";
            }

            std::string method_name() const
            {
                return _method_name;
            }

        protected:
            const std::string _method_name;
        };
    } // namespace errors
} // namespace dynamixel

#endif
