/** Evaluate round-trip time with DynamixelSDK.
 * This code is meant to be compiled within the Dynamixel SDK. It would not work with libdynamixel.
 * It is the complement of communication_timing.cpp used to compare the performance of both libraries.
 */

#include <fnctl.h>
#include <termios.h>
#define STDINF_FILENO 0

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <thread>

#include "dynamixel_sdk.h"

// Control table address
// #define ADDR_MX_TORQUE_ENABLE 24
// #define ADDR_MX_GOAL_POSITION 30
#define ADDR_MX_PRESENT_POSITION 36

// Protocol version
#define PROTOCOL_VERSION 1.0

// Default setting
#define DXL_ID 1
#define BAUDRATE 1000000
#define DEVICENAME "/dev/ttyUSB0"

#define TORQUE_ENABLE 1 // Value for enabling the torque
#define TORQUE_DISABLE 0 // Value for disabling the torque
#define DXL_MINIMUM_POSITION_VALUE 100
#define DXL_MAXIMUM_POSITION_VALUE 4000
#define DXL_MOVING_STATUS_THRESHOLD 10 // Dynamixel moving status threshold

#define ESC_ASCII_VALUE 0x1b

int getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

bool get_present_position(double& angle);

int main()
{
    // Initialize PortHandler instance
    // Set the port path
    // Get methods and members of PortHandlerLinux or PortHandlerWindows
    dynamixel::PortHandler* portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);

    // Initialize PacketHandler instance
    // Set the protocol version
    // Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
    dynamixel::PacketHandler* packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    int index = 0;
    int dxl_comm_result = COMM_TX_FAIL; // Communication result
    int dxl_goal_position[2] = {DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE}; // Goal position

    uint8_t dxl_error = 0; // Dynamixel error
    uint16_t dxl_present_position = 0; // Present position

    // Open port
    if (portHandler->openPort()) {
        printf("Succeeded to open the port!\n");
    }
    else {
        printf("Failed to open the port!\n");
        printf("Press any key to terminate...\n");
        getch();
        return 0;
    }

    // Set port baudrate
    if (portHandler->setBaudRate(BAUDRATE)) {
        printf("Succeeded to change the baudrate!\n");
    }
    else {
        printf("Failed to change the baudrate!\n");
        printf("Press any key to terminate...\n");
        getch();
        return 0;
    }

    // Enable Dynamixel Torque
    // dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
    // if (dxl_comm_result != COMM_SUCCESS) {
    //     printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
    // }
    // else if (dxl_error != 0) {
    //     printf("%s\n", packetHandler->getRxPacketError(dxl_error));
    // }
    // else {
    //     printf("Dynamixel has been successfully connected \n");
    // }

    bool success = false;
    double angle = 0;
    std::chrono::time_point<std::chrono::system_clock>
        start, end;

    while (1) {
        start = std::chrono::system_clock::now();
        success = get_present_position(angle);
        end = std::chrono::system_clock::now();
        std::cout << "present position of servo 1: "
                  //   << dynamixel::servos::Mx28::parse_present_position_angle(1, status)
                  << std::endl;
        std::chrono::duration<double> send_dt = end - start;
        // std::chrono::duration<double> recv_dt = recv_end - recv_start;
        // std::chrono::duration<double> reply_dt = recv_end - send_end;
        std::cout << "sending/receiving time: " << send_dt.count() << "s" << std::endl
                //   << "receiveing time: " << recv_dt.count() << "s" << std::endl
                //   << "reply time: " << reply_dt.count() << "s"
                  << std::endl;
        std::cout << "receive has " << (success ? "succeeded" : "failed")
                  << "\n"
                  << std::endl;
    }

    // Disable Dynamixel Torque
    // dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    // if (dxl_comm_result != COMM_SUCCESS) {
    //     printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
    // }
    // else if (dxl_error != 0) {
    //     printf("%s\n", packetHandler->getRxPacketError(dxl_error));
    // }

    // Close port
    portHandler->closePort();

    return 0;
}

bool get_present_position(double& angle)
{
    uint16_t dxl_present_position = 0;
    dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID,
        ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
        return false;
    }
    else if (dxl_error != 0) {
        // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
        return false;
    }

    // printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", DXL_ID, dxl_goal_position[index], dxl_present_position);
    angle = dxl_present_position * 2 * M_PI / 4095. + M_PI;
    return true;
}