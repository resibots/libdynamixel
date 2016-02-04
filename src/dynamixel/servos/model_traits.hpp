#ifndef DYNAMIXEL_SERVOS_MODEL_TRAITS_HPP_
#define DYNAMIXEL_SERVOS_MODEL_TRAITS_HPP_

namespace dynamixel {
namespace servos {
    template<typename Model>
    struct ModelTraits {        
    };

    // This traits define the protocol and control table of each model
    // It is expected to have a typedef called protocol_t and a struct
    // called CT, on which the addresses and sizes of the control table
    // are added as static const protocol_t::address_t and typedef (u)int8/16/32_t
    // respectively
}
}

#endif
