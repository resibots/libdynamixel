/* 
 * Monitor.cpp
 */
#include <iostream>
#include <cstdio>
#include "monitor.hpp"

Monitor::Monitor(Usb2Dynamixel controller,
                 std::vector<byte_t>& idVector,
                 int items,
                 float readDelay)
                 : 
                 controller(controller),
                 idVector(idVector),
                 items(items),
                 readDelay(readDelay) {

    // Init time
    timerclear(&timev_init);
    gettimeofday(&timev_init, NULL);
}

Monitor::Monitor(Usb2Dynamixel controller,
                 std::vector<byte_t>& idVector,
                 int items,
                 float readDelay,
                 struct timeval timev_init)
                 :
                 controller(controller),
                 idVector(idVector),
                 items(items),
                 readDelay(readDelay),
                 timev_init(timev_init) {
}

void Monitor::measure(void) {
    struct timeval timev_cur;   // Current absolute time
    struct timeval timev_rel;   // Current relative time (curent absolute time - initial time)

    gettimeofday(&timev_cur, NULL);
    timersub(&timev_cur, &timev_init, &timev_rel);

    fprintf(stderr, "%02u.%06u   ", (unsigned int) timev_rel.tv_sec, (unsigned int) timev_rel.tv_usec);

    for(unsigned int idIndex = 0 ; idIndex < idVector.size() ; idIndex++) {
        Status status;

        byte_t id = idVector[idIndex];
        byte_t param1 = 0x1e;
        byte_t param2 = 14;

        try {
            controller.send(ax12::ReadData(id, param1, param2));
            controller.recv(readDelay, status);

            if(items & GOAL_POSITION) {
                int goalPosition = (((int) status.get_params()[1]) << 8) + ((int) status.get_params()[0]);
                fprintf(stderr, "%4d ", goalPosition);
            }

            if(items & MOVING_SPEED) {
                int movingSpeed = (((int) status.get_params()[3]) << 8) + ((int) status.get_params()[2]);
                fprintf(stderr, "%4d ", movingSpeed);
            }

            if(items & TORQUE_LIMIT) {
                int torqueLimit = (((int) status.get_params()[5]) << 8) + ((int) status.get_params()[4]);
                fprintf(stderr, "%4d ", torqueLimit);
            }

            if(items & PRESENT_POSITION) {
                int presentPosition = (((int) status.get_params()[7]) << 8) + ((int) status.get_params()[6]);
                fprintf(stderr, "%4d ", presentPosition);
            }

            if(items & PRESENT_SPEED) {
                int presentSpeed = (((int) status.get_params()[9]) << 8) + ((int) status.get_params()[8]);
                fprintf(stderr, "%4d ", presentSpeed);
            }

            if(items & PRESENT_LOAD) {
                int presentLoad = (((int) status.get_params()[11]) << 8) + ((int) status.get_params()[10]);
                fprintf(stderr, "%4d ", presentLoad);
            }

            if(items & PRESENT_VOLTAGE) {
                int presentVoltage = (int) status.get_params()[12];
                fprintf(stderr, "%3d ", presentVoltage);
            }

            if(items & PRESENT_TEMPERATURE) {
                int presentTemperature = (int) status.get_params()[13];
                fprintf(stderr, "%3d ", presentTemperature);
            }

        } catch (Error& e) {
            std::cerr << "error : " << e.msg() << std::endl;
        }
    }

    fprintf(stderr, "\n");
}

void Monitor::close(void) {

}
