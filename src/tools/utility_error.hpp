#ifndef UTILITY_ERROR
#define UTILITY_ERROR

#include <dynamixel/errors.hpp>

#include <string>

namespace dynamixel {
    namespace errors {
        class UtilityError : public Error {
        public:
            UtilityError(const char* msg) : Error(msg) {}
            UtilityError(const std::string& msg) : Error(msg) {}
        };
    }
}

#endif // UTILITY_ERROR