#ifndef DYNAMIXEL_MISC_HPP_
#define DYNAMIXEL_MISC_HPP_

#include <sys/time.h>
namespace dynamixel {
    inline double get_time()
    {
        struct timeval tv;
        gettimeofday(&tv, 0x0);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
}

#endif
