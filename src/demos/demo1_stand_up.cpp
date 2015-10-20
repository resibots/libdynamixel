/*
 * Demo 1 : stand up
 */
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string.h>

#include <dynamixel/dynamixel.hpp>

using namespace dynamixel;

void usage(char * prog);

/**
    This programs will move a few actuators to a preset position and then
    make a few others turn continuously (wheel mode). The wheel actuator IDs
    are 4, 8, 12, 16. Also, the actuators seem to be assumed to have encoder
    values spanning from 0 to 1023 (probably AX-12 actuators, not MX series).
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

        // Config wheels //////////////////////////////////////////////////////
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

        // Config limbs position //////////////////////////////////////////////
        std::vector<byte_t> limbs_IDs;        // AX-12+ IDs
        std::vector<int> limbs_positions;     // Limbs positions
        std::vector<int> limbs_speeds;        // Limbs speeds (0x1 - 0x3ff)
        std::vector<bool> limbs_directions;   // Limbs directions (0 or 1)

        // Set limbs AX-12+ IDs : all excepts wheels IDs
        for (size_t i = 0 ; i < controller.ax12_ids().size() ; ++i)
        {
            if (std::find(wheels_IDs.begin(), wheels_IDs.end(), controller.ax12_ids()[i]) == wheels_IDs.end())
            {
                std::cout << "Limb ID : " << (int) controller.ax12_ids()[i] << std::endl;
                limbs_IDs.push_back(controller.ax12_ids()[i]);
                limbs_speeds.push_back(512);                // Set limbs speed (max = 0x3fe)
                limbs_directions.push_back(true);         // Set limbs directions (0 or 1)
                limbs_positions.push_back(512);           // Set limbs positions (max = 0x3fe)
            }
        }

        ax12::SetPositions set_positions(limbs_IDs, limbs_positions);
        controller.send(set_positions);
        usleep(1000); // Suspend execution for n microseconds

        ax12::SetSpeeds set_speeds(limbs_IDs, limbs_speeds, limbs_directions);
        controller.send(set_speeds);
        usleep(1000); // Suspend execution for n microseconds

        sleep(5);     // Suspend execution for n seconds
        controller.send(ax12::SetSpeeds(wheels_IDs, wheels_speeds, wheels_directions));

        while(true)
        {
            Status st;
            controller.recv(1000.5f, st);
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

