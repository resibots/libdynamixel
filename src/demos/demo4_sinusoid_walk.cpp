/*
 * Demo 4 : sinusoid walk
 */
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <dynamixel/dynamixel.hpp>

#include "actuator.hpp"
#include "monitor.hpp"

#define TICK_INTERVAL 10      // Nombre de us séparant deux appels
#define SPEED 150
#define READ_DURATION 0.02f
#define VERBOSE 0

using namespace dynamixel;

void usage(char * prog);

int main(int argc, char **argv) {

    // Getopt ///////////////////////////////////
    char controllerDevice[256] = "/dev/ttyUSB0";
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "d:")) != -1) {
        switch (c) {
            case 'd':
                strcpy(controllerDevice, optarg);
                break;
            case '?':
                usage(argv[0]);
                return 1;
        }
    }

    // Dynamixel //////////////////////////////////////////////////////////////////////////////////

    // Init ///////////////////////////////////////////////////////////////

    // Set device file
    Usb2Dynamixel controller(controllerDevice);

    // Scan actuators IDs
    controller.scan_ax12s();
    const std::vector<byte_t>& ax12_ids = controller.ax12_ids();
    if (!ax12_ids.size()) {
        std::cerr << "no ax12 detected, exit" << std::endl;
        return 0;
    }

    // Config actuators //////////////////////////////////////////////////////
    std::vector<Actuator *> actuators;        // AX-12+ IDs

    actuators.push_back(new Actuator( 1, 362,   40.0, 1.00,  0.00, 0, clockwise));
    actuators.push_back(new Actuator( 2, 362,  -40.0, 1.00, -M_PI / 2.0f, 0, counterclockwise));

    actuators.push_back(new Actuator( 5, 662,  -40.0, 1.00,  0.00, 0, counterclockwise));
    actuators.push_back(new Actuator( 6, 662,   40.0, 1.00, -M_PI / 2.0f, 0, clockwise));

    actuators.push_back(new Actuator( 9, 362,   40.0, 1.00,  0.00, 0, clockwise));
    actuators.push_back(new Actuator(10, 362,  -40.0, 1.00,  M_PI / 2.0f, 0, counterclockwise));

    actuators.push_back(new Actuator(13, 662,  -40.0, 1.00,  0.00, 0, counterclockwise));
    actuators.push_back(new Actuator(14, 662,   40.0, 1.00,  M_PI / 2.0f, 0, clockwise));

    if(VERBOSE) for(unsigned int i=0 ; i < actuators.size() ; i++) actuators[i]->printFunction();

    // Init timers //////////////////////////
    struct timeval timev_init;  // Initial absolute time (static)
    struct timeval timev_prev;  // Previous tick absolute time
    struct timeval timev_cur;   // Current absolute time
    struct timeval timev_rel;   // Current relative time (curent absolute time - initial time)
    struct timeval timev_diff;  // Current tick position (curent absolute time - previous tick time)

    timerclear(&timev_init);
    gettimeofday(&timev_init, NULL);

    // Init monitor /////////////////////////
    std::vector<byte_t> idMonitored;
    idMonitored.push_back(1);
    idMonitored.push_back(2);
    idMonitored.push_back(5);
    idMonitored.push_back(6);
    idMonitored.push_back(9);
    idMonitored.push_back(10);
    idMonitored.push_back(13);
    idMonitored.push_back(14);

    int items = Monitor::PRESENT_POSITION | Monitor::PRESENT_LOAD | Monitor::PRESENT_VOLTAGE | Monitor::PRESENT_TEMPERATURE;

    Monitor monitor(controller, idMonitored, items, READ_DURATION, timev_init);

    // Set speed ////////////////////////////
    for (size_t id_index = 0 ; id_index < actuators.size() ; ++id_index)
    {
        Status status;
        byte_t id = actuators[id_index]->getId();

        int moving_speed = SPEED;
        byte_t moving_speed_L = moving_speed & 0x00FF;
        byte_t moving_speed_H = (moving_speed & 0xFF00) >> 8;

        try {
            controller.send(ax12::WriteData(id, 0x20, moving_speed_L, moving_speed_H));
            controller.recv(READ_DURATION, status);
        } catch (Error& e) {
            std::cerr << "error : " << e.msg() << std::endl;
        }
    }

    if(VERBOSE) monitor.measure();

    timev_prev = timev_init;

    // Ticks loop ///////////////////////////
    while(true) {
        gettimeofday(&timev_cur, NULL);
        timersub(&timev_cur, &timev_prev, &timev_diff);

        // On fait un step de TICK_INTERVAL ms
        if(timev_diff.tv_usec > TICK_INTERVAL || timev_diff.tv_sec > 0) {

            // Calculate relative time //////////////////////////
            timersub(&timev_cur, &timev_init, &timev_rel);
            double t = timev_rel.tv_sec + (double) timev_rel.tv_usec / 1000000; // TODO : Approximative cast...

            // Move robot ///////////////////////////////////////
            // TODO: this should rather use dynamixel::ax12::SetPositions
            std::vector<byte_t> params;
            params.push_back(0x1e);
            params.push_back(0x02);

            for (size_t id_index = 0 ; id_index < actuators.size() ; ++id_index)
            {
                //Status status;
                byte_t id = actuators[id_index]->getId();

                // Calculate new positions
                int goal_position = actuators[id_index]->getInitialPosition()
                                    + actuators[id_index]->getAmplitude()
                                    * (actuators[id_index]->getDirection() ? 1 : -1)
                                    * sin(t * 2 * M_PI * actuators[id_index]->getAngularFrequency()
                                          + actuators[id_index]->getPhase());      // a * cos(w * t + d)
                byte_t goal_position_L = goal_position & 0x00FF;
                byte_t goal_position_H = (goal_position & 0xFF00) >> 8;

                params.push_back(id);
                params.push_back(goal_position_L);
                params.push_back(goal_position_H);
            }

            // Move actuator
            Status status;
            try {
                controller.send(ax12::SyncWrite(0xfe, params));
                //controller.recv(READ_DURATION, status);
            } catch (Error& e) {
                std::cerr << "error : " << e.msg() << std::endl;
            }

            if(VERBOSE) monitor.measure();

            // Update vars //////////////////////////////////////
            timev_prev = timev_cur;
        }
    }

    return 0;
}

void usage(char * prog) {
    fprintf(stderr, "usage: %s [-d controller_device]\n", prog);
}

