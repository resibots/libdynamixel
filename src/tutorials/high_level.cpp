#include <iostream>
#include <dynamixel/dynamixel.hpp>

int main(int argc, char** argv)
{
    // We need to know to which port/device our servos are connected
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " device/port" << std::endl;
        return -1;
    }

// Define protocol
#ifdef PROTOCOL1
    using protocol_t = dynamixel::protocols::Protocol1;
#else
    using protocol_t = dynamixel::protocols::Protocol2;
#endif

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

    // We want to catch exceptions
    try {
        // auto-detect scans all possible IDs and returns the connected servos
        std::vector<std::shared_ptr<dynamixel::servos::BaseServo<protocol_t>>> servos
            = dynamixel::auto_detect<protocol_t>(controller);

        // For each servo detected
        for (auto servo : servos) {
            // Print the name and id of the servo
            std::cout << "Detected an " << servo->model_name()
                      << " with ID " << servo->id() << std::endl;

            dynamixel::StatusPacket<protocol_t> st;
            // Enable torque
            controller.send(servo->set_torque_enable(1));
            controller.recv(st);

            // Put it in position of pi
            controller.send(servo->set_goal_position_angle(M_PI));
            controller.recv(st);

            // Sleep for a bit to allow the servo to reach the position
            sleep(1);

            // Request the current position of the servo
            controller.send(servo->get_present_position_angle());
            controller.recv(st);

            // Parse the position from the packet received
            if (st.valid()) // check if valid response
            {
                double a = servo->parse_present_position_angle(st);
                std::cout << "It went to the angle: " << a << ". "
                          << "Error: " << std::sqrt((a - M_PI) * (a - M_PI)) << std::endl;
            }
            else
                std::cout << "Could not read the angle of the servo." << std::endl;
        }
    }
    catch (const dynamixel::errors::Error& e) {
        // Something bad happened
        std::cerr << "Error (dynamixel): " << e.msg() << std::endl;
    }

    return 0;
}
