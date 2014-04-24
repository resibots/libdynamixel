#include <iostream>
#include <cstring>
#include <sstream>

#include <cstdio>
#include "usb2dynamixel.hpp"
#include "ax12.hpp"

namespace dynamixel
{
  void Usb2Dynamixel::open_serial(const std::string& name, int baudrate)
  {
    struct termios tio_serial;

    _fd = open(name.c_str(), O_RDWR | O_NOCTTY);
    if (_fd == -1)
      throw Error("error opening device:" + name + std::string(strerror(errno)));

    // Serial port setting
    bzero(&tio_serial, sizeof(tio_serial));
    tio_serial.c_cflag = CS8 | CLOCAL | CREAD;
    tio_serial.c_iflag = IGNBRK | IGNPAR;
    tio_serial.c_oflag = 0;
    tio_serial.c_lflag = 0;

    // could be 1
    tio_serial.c_cc[VMIN] = 0;

    cfsetispeed(&tio_serial, baudrate);
    cfsetospeed(&tio_serial, baudrate);
    //cfsetispeed(&tio_serial, B115200);
    //cfsetospeed(&tio_serial, B115200);

    int res = cfgetispeed(&tio_serial);
    tcflush(_fd, TCIFLUSH);
    tcsetattr(_fd, TCSANOW, &tio_serial);
  }

  void Usb2Dynamixel::send(const Instruction& inst)
  {
    if (_fd == -1)
      return;
    int ret = write(_fd, (byte_t*)inst.packet(), inst.packet_size());
    //    for (size_t i = 0; i < inst.packet_size(); ++i)
    //      std::cout<<(int)inst.packet()[i]<<std::endl;
    if (ret != inst.packet_size())
      {
	std::stringstream ofs;
	perror("write:");
	ofs << "written= " << ret << " size="<<inst.packet_size();
	throw Error("Usb2Dynamixel::Send: packet not fully written" + ofs.str());
      }
  }

  bool Usb2Dynamixel::recv(float timeout, Status& st)
  {
    if (_fd == -1)
      return true;
    int p = 0;
    double time = get_time();
    bool done = false;
    memset(_recv_buffer, 0, _recv_buffer_size);
    do
      {
	double current_time = get_time();
	byte_t b;
	int res = read(_fd, &b, 1);
	if (res > 0)
	  {
	    _recv_buffer[p++] = b;
	    time = current_time;
	    done = st.decode_packet(_recv_buffer, p);
	  }
	if (current_time - time > timeout)
	  return false;
      }
    while (!done);
    return true;
  }

  void Usb2Dynamixel::scan_ax12s()
  {
    if (_fd == -1)
      return;
    Status st;
    for (size_t i = 0; i < 100; ++i)
      {
	send(ax12::Ping(i));
	if(recv(0.01f, st))//TODO tune this number
	  _ax12_ids.push_back(st.get_id());
      }
  }
}
