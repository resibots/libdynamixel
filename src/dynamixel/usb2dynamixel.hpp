#ifndef USB2DYNAMIXEL_FTDI_HPP_
#define USB2DYNAMIXEL_FTDI_HPP_

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <vector>
#include "error.hpp"
#include "misc.hpp"
#include "instruction.hpp"
#include "status.hpp"

namespace dynamixel
{
  static const byte_t broadcast = 0xfe;

  class Usb2Dynamixel
  {
  public:
    Usb2Dynamixel(const std::string& name, int baudrate = B115200)
    { open_serial(name, baudrate); }

    Usb2Dynamixel() : _fd(-1) {}

    void open_serial(const std::string& name, int baudrate = B115200);

    // general send
    void send(const Instruction& inst);
    void close_serial(){close(_fd);_fd=-1;}
    bool isOpen()
    {
        return !(_fd==-1);
    }
    void flush()
        {
            tcflush(_fd,TCIFLUSH);
        }
    // general receive
    bool recv(float timeout, Status& status);

    // ax12 specifics
    void scan_ax12s();

    const std::vector<byte_t>& ax12_ids() const  { return _ax12_ids; }

  private:
    int _fd;
    static const size_t _recv_buffer_size = 256;
    byte_t _recv_buffer[_recv_buffer_size];
    std::vector<byte_t> _ax12_ids;
  };
}

#endif
