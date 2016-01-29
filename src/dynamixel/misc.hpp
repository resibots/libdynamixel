#ifndef DYNAMIXEL_MISC_HPP_
#define DYNAMIXEL_MISC_HPP_

#include <sys/time.h>
#include <termios.h>
#include <sstream>

#include "error.hpp"

namespace dynamixel
{
  typedef unsigned char byte_t;

  inline double get_time()
  {
    struct timeval tv;
    gettimeofday(&tv, 0x0);
    return tv.tv_sec + tv.tv_usec * 1e-6;
  }

  inline int get_baudrate(const unsigned int baudrate)
  {
    int os_baudrate;

    switch(baudrate)
    {
      case 0:
          os_baudrate = B0;
          break;		/* hang up */
      case 50:
          os_baudrate = B50;
          break;
      case 75:
          os_baudrate = B75;
          break;
      case 110:
          os_baudrate = B110;
          break;
      case 134:
          os_baudrate = B134;
          break;
      case 150:
          os_baudrate = B150;
          break;
      case 200:
          os_baudrate = B200;
          break;
      case 300:
          os_baudrate = B300;
          break;
      case 600:
          os_baudrate = B600;
          break;
      case 1200:
          os_baudrate = B1200;
          break;
      case 1800:
          os_baudrate = B1800;
          break;
      case 2400:
          os_baudrate = B2400;
          break;
      case 4800:
          os_baudrate = B4800;
          break;
      case 9600:
          os_baudrate = B9600;
          break;
      case 19200:
          os_baudrate = B19200;
          break;
      case 38400:
          os_baudrate = B38400;
          break;
      case 57600:
          os_baudrate = B57600;
          break;
      case 115200:
          os_baudrate = B115200;
          break;
      case 230400:
          os_baudrate = B230400;
          break;
      case 460800:
          os_baudrate = B460800;
          break;
      case 500000:
          os_baudrate = B500000;
          break;
      case 576000:
          os_baudrate = B576000;
          break;
      case 921600:
          os_baudrate = B921600;
          break;
      case 1000000:
          os_baudrate = B1000000;
          break;
      case 1152000:
          os_baudrate = B1152000;
          break;
      case 1500000:
          os_baudrate = B1500000;
          break;
      case 2000000:
          os_baudrate = B2000000;
          break;
      case 2500000:
          os_baudrate = B2500000;
          break;
      case 3000000:
          os_baudrate = B3000000;
          break;
      case 3500000:
          os_baudrate = B3500000;
          break;
      case 4000000:
          os_baudrate = B4000000;
          break;
      default:
        os_baudrate = B1000000;
        std::stringstream err_message;
        err_message << "Invalid baudrate: " << os_baudrate << "! Setting to default: 1,000,000 bauds";
        throw Error(err_message.str());
    }
    return os_baudrate;
  }
}

#endif