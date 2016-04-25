#include <dynamixel/errors.hpp>

namespace dynamixel {
    namespace errors {
        class MissingParameter : public Error {
        public:
            MissingParameter(std::string command,
                const std::vector<std::string>& parameters)
            {
                std::stringstream err_message;
                err_message << "The " << command << " command needs at least "
                            << "the following parameters:" << std::endl;

                for (auto parameter : parameters) {
                    err_message << "- " << parameter << "\n";
                }

                this->_msg = err_message.str();
            }
        };
    }
}