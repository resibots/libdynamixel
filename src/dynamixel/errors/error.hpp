#ifndef DYNAMIXEL_ERROR_HPP_
#define DYNAMIXEL_ERROR_HPP_

#ifdef USE_FTDI
#include <ftdi.h>
#endif

#include <string>

namespace dynamixel {
    namespace errors {
        class Error {
        public:
            Error() {}
            Error(const char* msg) : _msg(msg) {}
            Error(const std::string& msg) : _msg(msg) {}
#ifdef USE_FTDI
            Error(const char* msg, struct ftdi_context* ftdic)
                : _msg(std::string(msg) + std::string(": ") + std::string(ftdi_get_error_string(ftdic)))
            {
            }
#endif
            const std::string& msg() const
            {
                return _msg;
            }

            virtual std::ostream& print(std::ostream& os) const
            {
                os << _msg;

                return os;
            }

        protected:
            std::string _msg;
        };

        inline void check(const char* file, int line, bool value,
            const std::string& msg)
        {
            if (!value)
                throw Error(msg + std::string("[") + std::string(file) + "]");
        }

        inline std::ostream& operator<<(std::ostream& os, const Error& err)
        {
            return err.print(os);
        }
    }
}

#define CHECK(val, msg) check(__FILE__, __LINE__, (val), msg)
#endif
