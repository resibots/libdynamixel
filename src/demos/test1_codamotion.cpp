#include <iostream>
#include <algorithm>
#include <cstdio>

#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <dynamixel/dynamixel.hpp>

#define TICK_INTERVAL 1000000      // Nombre de us séparant deux appels
//#define TICK_INTERVAL 500000      // Nombre de us séparant deux appels
#define NB_AX12 8
#define NB_STEPS 8
#define DEFAULT_SPEED 150
#define READ_DURATION 0.05f

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
    } else {
        std::cout << ax12_ids.size() << " ax12 are connected" << std::endl;
    }

    // Config actuators //////////////////////////////////////////////////////
    
    // AX-12+ IDs
    byte_t actuators_IDs[NB_AX12] = {1, 2, 5, 6, 9, 10, 13, 14};

    int actuators_positions[NB_AX12][NB_STEPS] = {{ 512, 312, 512, 712, 512, 312, 512, 712},
						  { 512, 512, 512, 512, 512, 312, 512, 712},
						  { 512, 512, 512, 512, 512, 512, 512, 512},
						  { 512, 512, 512, 512, 512, 512, 512, 512},
						  { 512, 512, 512, 512, 512, 512, 512, 512},
						  { 512, 512, 512, 512, 512, 512, 512, 512},
						  { 512, 512, 512, 512, 512, 512, 512, 512},
						  { 512, 512, 512, 512, 512, 512, 512, 512}};                        

    Status status;

    // Init timers //////////////////////////
    struct timeval timev_init;  // Initial absolute time (static)
    struct timeval timev_prev;  // Previous tick absolute time
    struct timeval timev_cur;   // Current absolute time
    struct timeval timev_rel;   // Current relative time (curent absolute time - initial time)
    struct timeval timev_diff;  // Current tick position (curent absolute time - previous tick time)
   
    timerclear(&timev_init);
    gettimeofday(&timev_init, NULL);

    int step_index = 0;

    // First tick ///////////////////////////
    for (size_t id_index = 0 ; id_index < NB_AX12 ; ++id_index)
    {
        byte_t id = actuators_IDs[id_index];

        //int moving_speed = actuators_speeds[id_index][0];
        int moving_speed = DEFAULT_SPEED;
        byte_t moving_speed_L = moving_speed & 0x00FF;
        byte_t moving_speed_H = (moving_speed & 0xFF00) >> 8;

        int goal_position = actuators_positions[id_index][0];
        byte_t goal_position_L = goal_position & 0x00FF;
        byte_t goal_position_H = (goal_position & 0xFF00) >> 8;

        try {
            controller.send(ax12::WriteData(id, 0x20, moving_speed_L, moving_speed_H));
            controller.recv(READ_DURATION, status);

            controller.send(ax12::WriteData(id, 0x1e, goal_position_L, goal_position_H));
            controller.recv(READ_DURATION, status);
        } catch (Error& e) {
            std::cerr << "error : " << e.msg() << std::endl;
        }
    }

    timev_prev = timev_init;

    // Ticks loop ///////////////////////////
    while(true) {
        gettimeofday(&timev_cur, NULL);
        timersub(&timev_cur, &timev_prev, &timev_diff);

        // On fait un step de TICK_INTERVAL ms
        if(timev_diff.tv_usec > TICK_INTERVAL || timev_diff.tv_sec > 0) {

            // Calculate relative time //////////////////////////
            timersub(&timev_cur, &timev_init, &timev_rel);

            // Calculate step_index value ////////////////////////////
            step_index = (step_index + 1) % NB_STEPS;

            // Move robot ///////////////////////////////////////
            for (size_t id_index = 0 ; id_index < sizeof(actuators_IDs)  / sizeof(actuators_IDs[0]) ; ++id_index)
            {
                byte_t id = actuators_IDs[id_index];

                /*
                int moving_speed = actuators_speeds[id_index][step_index];
                byte_t moving_speed_L = moving_speed & 0x00FF;
                byte_t moving_speed_H = (moving_speed & 0xFF00) >> 8;
                */

                int goal_position = actuators_positions[id_index][step_index];
                byte_t goal_position_L = goal_position & 0x00FF;
                byte_t goal_position_H = (goal_position & 0xFF00) >> 8;

                try {
                    //controller.send(ax12::WriteData(id, 0x20, moving_speed_L, moving_speed_H));
                    //controller.recv(READ_DURATION, status);

                    controller.send(ax12::WriteData(id, 0x1e, goal_position_L, goal_position_H));
                    controller.recv(READ_DURATION, status);
                } catch (Error& e) {
                    std::cerr << "error : " << e.msg() << std::endl;
                }
            }

            // Update vars //////////////////////////////////////
            timev_prev = timev_cur;
        }
    }

    return 0;
}

void usage(char * prog) {
    fprintf(stderr, "usage: %s [-d controller_device]\n", prog);
}

