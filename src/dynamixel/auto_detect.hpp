#ifndef DYNAMIXEL_AUTO_DETECT_HPP_
#define DYNAMIXEL_AUTO_DETECT_HPP_

#include <memory>
#include <map>

#include "errors/error.hpp"
#include "servos.hpp"

namespace dynamixel {
    inline std::shared_ptr<servos::BaseServo<protocols::Protocol1>> get_servo(protocols::Protocol1::id_t id, uint16_t model, protocols::Protocol1::address_t selected_protocol)
    {
        switch (model) {
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

    inline std::shared_ptr<servos::BaseServo<protocols::Protocol2>> get_servo(protocols::Protocol2::id_t id, uint16_t model, protocols::Protocol2::address_t selected_protocol)
    {
        switch (model) {
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
        case servos::ProH54200S500::ct_t::model_number_value:
            return std::make_shared<servos::ProH54200S500>(id);
        case servos::ProH54100S500::ct_t::model_number_value:
            return std::make_shared<servos::ProH54100S500>(id);
        case servos::ProH4220S300::ct_t::model_number_value:
            return std::make_shared<servos::ProH4220S300>(id);
        case servos::ProM5460S250::ct_t::model_number_value:
            return std::make_shared<servos::ProM5460S250>(id);
        case servos::ProM5440S250::ct_t::model_number_value:
            return std::make_shared<servos::ProM5440S250>(id);
        case servos::ProM4210S260::ct_t::model_number_value:
            return std::make_shared<servos::ProM4210S260>(id);
        default:
            throw errors::Error("Unrecognized model number");
        }
    }

    /** Send a ping to an ID. If it is answered, read the actuator model and
        instanciate a class of the correct type.

        @param id value, from 1 to 254 identifiying an actuator

        @return std::shared_ptr to a servo object if we got an answer
    **/
    template <typename Protocol, typename Controller>
    inline std::shared_ptr<servos::BaseServo<Protocol>>
    find_servo(const Controller& controller, typename Protocol::id_t id)
    {
        // Dummy variable used only to differenciate between the two version of
        // get_servo (protocol 1 or 2)
        typename Protocol::address_t selected_protocol = 0;

        try {
            // send a ping.
            controller.send(instructions::Ping<Protocol>(id));
            StatusPacket<Protocol> status;

            if (controller.recv(status) && status.id() == id) {
                // get actuator model
                controller.send(instructions::Read<Protocol>(id, 0, 2));
                if (controller.recv(status)) {
                    uint16_t model;
                    Protocol::unpack_data(status.parameters(), model);
                    return get_servo(id, model, selected_protocol);
                }
            }
        }
        catch (const errors::Error&) {
            // TODO: This behavior is "dangerous". We should at least tell the
            //       user something is happening, or give the option for this
            //       function to re-raise exceptions
        }

        return std::shared_ptr<servos::BaseServo<Protocol>>();
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
    inline std::vector<std::shared_ptr<servos::BaseServo<Protocol>>>
    auto_detect(const Controller& controller)
    {
        using ServoPtr = std::shared_ptr<servos::BaseServo<Protocol>>;

        // vector of actuators returned by this function
        std::vector<ServoPtr> res;

        // search through each possible device ID
        for (typename Protocol::id_t id = 0; id < Protocol::broadcast_id; id++) {
            ServoPtr servo = find_servo<Protocol>(controller, id);
            if (servo)
                res.push_back(servo);
        }

        return res;
    }

    /** Auto-detect all connected actuators using a given protocol

        @see template <typename Protocol, typename Controller>
            inline std::vector<std::shared_ptr<servos::BaseServo<Protocol>>>
            auto_detect(const Controller& controller)

        @param controller object handling the USB to dynamixel interface, instance
            of the dynamixel::controllers::Usb2Dynamixel class
        @param ids vector of ids (1-254) that will be searched
        @return vector of actuators
    **/
    template <typename Protocol, typename Controller>
    inline std::vector<std::shared_ptr<servos::BaseServo<Protocol>>>
    auto_detect(
        const Controller& controller,
        const std::vector<typename Protocol::id_t>& ids)
    {
        using ServoPtr = std::shared_ptr<servos::BaseServo<Protocol>>;

        // vector of actuators returned by this function
        std::vector<ServoPtr> res;

        // search through each possible device ID
        for (typename Protocol::id_t id : ids) {
            ServoPtr servo = find_servo<Protocol>(controller, id);
            if (servo)
                res.push_back(servo);
        }

        return res;
    }

    /** Auto-detect all connected actuators using a given protocol.

        This function does the same as auto_detect but instead of returning a
        vector, it gives a map from ID to object.

        @see auto_detect

        @param controller object handling the USB to dynamixel interface, instance
            of the dynamixel::controllers::Usb2Dynamixel class
        @return vector of actuators
        @throws dynamixel::errors::Errors if there is a problem during send
        @throws dynamixel::errors::UnpackError if the size of received packet is
            not correct
    **/
    template <typename Protocol, typename Controller>
    inline std::map<typename Protocol::id_t, std::shared_ptr<servos::BaseServo<Protocol>>>
    auto_detect_map(const Controller& controller)
    {
        using ServoPtr = std::shared_ptr<servos::BaseServo<Protocol>>;

        // vector of actuators returned by this function
        std::map<typename Protocol::id_t, ServoPtr> res;

        // search through each possible device ID
        for (typename Protocol::id_t id = 0; id < Protocol::broadcast_id; id++) {
            ServoPtr servo = find_servo<Protocol>(controller, id);
            if (servo)
                res[id] = servo;
        }

        return res;
    }

    /** Auto-detect all connected actuators using a given protocol.

        @see template <typename Protocol, typename Controller>
            inline std::map<typename Protocol::id_t, std::shared_ptr<servos::BaseServo<Protocol>>>
            auto_detect_map(const Controller& controller)

        @param controller object handling the USB to dynamixel interface, instance
            of the dynamixel::controllers::Usb2Dynamixel class
        @return vector of actuators
        @throws dynamixel::errors::Errors if there is a problem during send
        @throws dynamixel::errors::UnpackError if the size of received packet is
            not correct
    **/
    template <typename Protocol, typename Controller>
    inline std::map<typename Protocol::id_t, std::shared_ptr<servos::BaseServo<Protocol>>>
    auto_detect_map(
        const Controller& controller,
        const std::vector<typename Protocol::id_t>& ids)
    {
        using ServoPtr = std::shared_ptr<servos::BaseServo<Protocol>>;

        // vector of actuators returned by this function
        std::map<typename Protocol::id_t, ServoPtr> res;

        // search through each possible device ID
        for (typename Protocol::id_t id : ids) {
            ServoPtr servo = find_servo<Protocol>(controller, id);
            if (servo)
                res[id] = servo;
        }

        return res;
    }
} // namespace dynamixel

#endif
