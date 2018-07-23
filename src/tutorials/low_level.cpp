#include <iostream>
#include <dynamixel/dynamixel_core.hpp>
#include <dynamixel/servos/mx106.hpp>
#include <dynamixel/servos/mx28.hpp>

int main(int argc, char** argv)
{
    // We need to know to which port/device our servos are connected
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " device/port" << std::endl;
        return -1;
    }

    // Create a new Usb2Dynamixel controller with default parameters
    dynamixel::controllers::Usb2Dynamixel controller;

    // Always use try-catch when connecting to a device/port
    try {
        // connect to device/port
        controller.open_serial(argv[1]);
    }
    catch (const dynamixel::errors::Error& e) {
        // Something bad happened
        std::cerr << "Error (dynamixel): " << e.msg() << std::endl;
        return 0;
    }

    // Enable throwing exceptions -- by default all errors are ignored/silenced
    controller.set_report_bad_packet(true);

    // We need to know the IDs and the type of actuators
    std::vector<size_t> mx_106_ids = {1, 2}; // we have 2 MX106 with IDs 1 and 2
    std::vector<size_t> mx_28_ids = {3, 4}; // we have 2 MX28 with IDs 3 and 4

    using protocol_t = dynamixel::protocols::Protocol1;

    // We want to catch exceptions
    try {
        dynamixel::StatusPacket<protocol_t> status;

        for (size_t i = 0; i < mx_106_ids.size(); i++) {
            using mx106_t = dynamixel::servos::Mx106;
            // Enable torque
            controller.send(mx106_t::set_torque_enable(mx_106_ids[i], 1));
            controller.recv(status);

            // Set the moving speed of the actuator
            controller.send(mx106_t::set_moving_speed(mx_106_ids[i], 50));
            controller.recv(status);

            // Write goal position in register of the actuator
            controller.send(mx106_t::reg_goal_position_angle(mx_106_ids[i], M_PI));
            controller.recv(status);
        }

        for (size_t i = 0; i < mx_28_ids.size(); i++) {
            using mx28_t = dynamixel::servos::Mx28;
            // Enable torque
            controller.send(mx28_t::set_torque_enable(mx_28_ids[i], 1));
            controller.recv(status);

            // Set the moving speed of the actuator
            controller.send(mx28_t::set_moving_speed(mx_28_ids[i], 50));
            controller.recv(status);

            // Write goal position in register of the actuator
            controller.send(mx28_t::reg_goal_position_angle(mx_28_ids[i], M_PI));
            controller.recv(status);
        }

        // Send broadcast action
        controller.send(dynamixel::instructions::Action<protocol_t>(protocol_t::broadcast_id));
    }
    catch (const dynamixel::errors::Error& e) {
        // Something bad happened
        std::cerr << "Error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
