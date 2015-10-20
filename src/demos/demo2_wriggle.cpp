/*
 * Demo 2 :  wriggle
 */
#include <iostream>
#include <algorithm>

#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <cstdio>

#include <dynamixel/dynamixel.hpp>

#define TICK_INTERVAL 100000      // Nombre de us séparant deux appels
#define DEFAULT_SPEED 150
#define READ_DURATION 0.05f

using namespace dynamixel;

void usage(char * prog);

/**
    This program will activate a number of dynamixels considered to be wheels
    and move the other ones in a cosine trajectory.
    Actuators assumed to be wheels: 4, 8, 12, 16. All the others are considered
    to be in joint mode and controlled as such.
 */
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

    // Dynamixel ////////////////////////////////
    try
    {
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

        std::vector<byte_t> wheels_IDs;        // AX-12+ IDs
        std::vector<int> wheels_speeds;        // Wheels speeds (0x1 - 0x3ff)
        std::vector<bool> wheels_directions;   // Wheels directions (0 or 1)

        // Set wheels AX-12+ IDs : [4, 8, 12, 16]
        wheels_IDs.push_back(4);
        wheels_IDs.push_back(8);
        wheels_IDs.push_back(12);
        wheels_IDs.push_back(16);

        // Set endless turn mode for wheels (p.17)
        for (size_t i = 0; i < wheels_IDs.size(); ++i)
        {
            controller.send(ax12::SetContinuous(wheels_IDs[i]));  // Set endless turn mode for wheels (p.17)
            usleep(1000);                                  // Suspend execution for n microseconds
            wheels_speeds.push_back(0x0fe);                // Set wheels speed (max = 0x3fe)
        }

        // Set wheels directions
        wheels_directions.push_back(true);  // Wheel #4 get left
        wheels_directions.push_back(false); // Wheel #8 get right
        wheels_directions.push_back(false); // Wheel #12 get right
        wheels_directions.push_back(true);  // Wheel #16 get left

        // Get status
        Status st;
        controller.recv(0.5f, st); // get status (timeout = 0.5 sec)


        // Config actuators //
        std::vector<byte_t> actuators_IDs;        // AX-12+ IDs
        std::vector<int> actuators_positions;     // Actuator positions
        std::vector<int> actuators_speeds;        // Actuator speeds (0x1 - 0x3ff)
        std::vector<bool> actuators_directions;   // Actuator directions (0 or 1)

        // Set AX-12+ IDs : [1, 2, 3]
        actuators_IDs.push_back(1);
        actuators_IDs.push_back(2);
        actuators_IDs.push_back(5);
        actuators_IDs.push_back(6);
        actuators_IDs.push_back(9);
        actuators_IDs.push_back(10);
        actuators_IDs.push_back(13);
        actuators_IDs.push_back(14);

        Status status;

        // Init timers //////////////////////////
        struct timeval timev_init;  // Initial absolute time (static)
        struct timeval timev_prev;  // Previous tick absolute time
        struct timeval timev_cur;   // Current absolute time
        struct timeval timev_rel;   // Current relative time (curent absolute time - initial time)
        struct timeval timev_diff;  // Current tick position (curent absolute time - previous tick time)

        timerclear(&timev_init);
        gettimeofday(&timev_init, NULL);

        // First tick ///////////////////////////
        for (size_t i = 0 ; i < actuators_IDs.size() ; ++i)
        {
            actuators_speeds.push_back(DEFAULT_SPEED);               // Set actuators speed (max = 0x3fe)
            actuators_directions.push_back(true);         // Set actuators directions (0 or 1)
            actuators_positions.push_back(512);           // Set actuators positions (max = 0x3fe)
        }

        ax12::SetSpeeds set_speeds(actuators_IDs, actuators_speeds, actuators_directions);
        controller.send(set_speeds);
        usleep(1000); // Suspend execution for n microseconds

        ax12::SetPositions set_positions(actuators_IDs, actuators_positions);
        controller.send(set_positions);
        usleep(1000); // Suspend execution for n microseconds

        controller.recv(READ_DURATION, status);

        timev_prev = timev_init;
        int oldPosition = 512;

        // Ticks loop ///////////////////////////
        while(true) {
            gettimeofday(&timev_cur, NULL);
            timersub(&timev_cur, &timev_prev, &timev_diff);

            // On fait un step de TICK_INTERVAL ms
            if(timev_diff.tv_usec > TICK_INTERVAL || timev_diff.tv_sec > 0) {

                // Calculate relative time //////////////////////////
                timersub(&timev_cur, &timev_init, &timev_rel);

                // Calculate new positions //////////////////////////
                double t = timev_rel.tv_sec + (double) timev_rel.tv_usec / 1000000; // TODO : Approximative cast...

                int newPosition = 512 + 60.0 * cos(t * 2 * M_PI * 1.0 + 3.14);      // a * cos(w * t + d)
                fprintf(stderr, "%02u.%06u : %4d\n", (unsigned int) timev_rel.tv_sec, (unsigned int) timev_rel.tv_usec, newPosition);

                // Move robot ///////////////////////////////////////
                actuators_directions.clear();
                actuators_positions.clear();

                for (size_t i = 0 ; i < actuators_IDs.size() ; ++i)
                {
                    actuators_directions.push_back(newPosition > oldPosition ? true : false);   // Set actuators directions (0 or 1)
                    actuators_positions.push_back(newPosition);                                 // Set actuators positions (max = 0x3fe)
                }

                ax12::SetPositions set_positions(actuators_IDs, actuators_positions);
                controller.send(set_positions);

                controller.recv(READ_DURATION, status);

                // Update vars //////////////////////////////////////
                timev_prev = timev_cur;
                oldPosition = newPosition;
            }
        }
    }
    catch (Error& e)
    {
        std::cerr << "error : " << e.msg() << std::endl;
    }

    return 0;
}

void usage(char * prog) {
    fprintf(stderr, "usage: %s [-d controller_device]\n", prog);
}

