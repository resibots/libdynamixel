#ifndef DYNAMIXEL_AUTO_DETECT_HPP_
#define DYNAMIXEL_AUTO_DETECT_HPP_

#include <memory>

#include "errors/error.hpp"
#include "servos.hpp"

namespace dynamixel {
    inline std::shared_ptr<servos::BaseServo<protocols::Protocol1> > get_servo(protocols::Protocol1::address_t id, uint16_t model)
    {
        switch(model)
        {
            case servos::Ax12::ct_t::model_number_value:
                return std::make_shared<servos::Ax12>(id);
            case servos::Ax12W::ct_t::model_number_value:
                return std::make_shared<servos::Ax12W>(id);
            case servos::Ax18::ct_t::model_number_value:
                return std::make_shared<servos::Ax18>(id);
            case servos::Ex106::ct_t::model_number_value:
                return std::make_shared<servos::Ex106>(id);
            case servos::Mx12::ct_t::model_number_value:
                return std::make_shared<servos::Mx12>(id);
            case servos::Mx28::ct_t::model_number_value:
                return std::make_shared<servos::Mx28>(id);
            case servos::Mx64::ct_t::model_number_value:
                return std::make_shared<servos::Mx64>(id);
            case servos::Mx106::ct_t::model_number_value:
                return std::make_shared<servos::Mx106>(id);
            default:
                throw errors::Error("Unrecognized model number");
        }
    }

    inline std::shared_ptr<servos::BaseServo<protocols::Protocol2> > get_servo(protocols::Protocol2::address_t id, uint16_t model)
    {
        switch(model)
        {
            case servos::Xl320::ct_t::model_number_value:
                return std::make_shared<servos::Xl320>(id);
            case servos::ProL4210S300::ct_t::model_number_value:
                return std::make_shared<servos::ProL4210S300>(id);
            case servos::ProL5430S400::ct_t::model_number_value:
                return std::make_shared<servos::ProL5430S400>(id);
            case servos::ProL5430S500::ct_t::model_number_value:
                return std::make_shared<servos::ProL5430S500>(id);
            case servos::ProL5450S290::ct_t::model_number_value:
                return std::make_shared<servos::ProL5450S290>(id);
            case servos::ProL5450S500::ct_t::model_number_value:
                return std::make_shared<servos::ProL5450S500>(id);
            default:
                throw errors::Error("Unrecognized model number");
        }
    }

    /** Auto-detect all connected actuators using a given protocol.

        The template parameter Controller is inferred from the function's
        parameter. Conversely, it is compulsory to specify the protocol version:
        either dynamixel::protocols::Protocol1 or dynamixel::protocols::Protocol2.

        The returned vector contains objects corresponding to the actuators that
        were found. You can use these objects to do all actions that are possible
        with the Dynamixel actuators.

        @param controller object handling the USB to dynamixel interface, instance
            of the dynamixel::controllers::Usb2Dynamixel class
        @return vector of actuators
    **/
    template <typename Protocol, typename Controller>
    inline std::vector<std::shared_ptr<servos::BaseServo<Protocol> > > auto_detect(const Controller& controller)
    {
        // vector of actuators returned by this function
        std::vector<std::shared_ptr<servos::BaseServo<Protocol> > > res;

        // search through each possible device ID
        for (typename Protocol::address_t id = 0; id < Protocol::broadcast_id; id++) {
            try {
                // Send a ping. If it is answered, read the actuator model and
                // instanciate a class of the correct type
                controller.send(instructions::Ping<Protocol>(id));
                StatusPacket<Protocol> status;
                if (controller.recv(status) && status.id() == id) {
                    // get actuator model
                    controller.send(instructions::Read<Protocol>(id, 0, 2));
                    if (controller.recv(status)) {
                        uint16_t model;
                        Protocol::unpack_data(status.parameters(), model);
                        res.push_back(get_servo(id, model));
                    }
                }
            }
            catch (const errors::Error&) {
                continue;
            }
        }

        return res;
    }
}

#endif
