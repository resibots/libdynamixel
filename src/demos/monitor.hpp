#include <dynamixel/dynamixel.hpp>
#include <sys/time.h>

using namespace dynamixel;

class Monitor {

protected :
    Usb2Dynamixel controller;

    std::vector<byte_t>& idVector;

    int items;

    float readDelay;

    struct timeval timev_init;

public :
    const int static GOAL_POSITION = 1;            // 00000001 mask
    const int static MOVING_SPEED = 1 << 1;        // 00000010 mask
    const int static TORQUE_LIMIT = 1 << 2;        // 00000100 mask
    const int static PRESENT_POSITION = 1 << 3;    // 00001000 mask
    const int static PRESENT_SPEED = 1 << 4;       // 00010000 mask
    const int static PRESENT_LOAD = 1 << 5;        // 00100000 mask
    const int static PRESENT_VOLTAGE = 1 << 6;     // 01000000 mask
    const int static PRESENT_TEMPERATURE = 1 << 7; // 10000000 mask

    Monitor(Usb2Dynamixel controller, std::vector<byte_t>& idVector, int items, float readDelay);

    Monitor(Usb2Dynamixel controller, std::vector<byte_t>& idVector, int items, float readDelay, struct timeval timev_init);

    void measure(void);

    void close(void);

};

