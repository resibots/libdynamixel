#include <iostream>
#include <dynamixel/dynamixel.hpp>
#include <cmath>
#include <map>
#include <boost/assign/list_of.hpp> // for 'map_list_of()'

#define READ_DURATION 0.02f

int main()
{
  try
  {
    using namespace dynamixel;
    dynamixel::Usb2Dynamixel controller;
    controller.open_serial("/dev/ttyUSB0");

     // Scan actuators IDs
    controller.scan_ax12s();
    const std::vector<byte_t>& ax12_ids = controller.ax12_ids();
    std::cout << "[ax12] " << ax12_ids.size() << " ax12 are connected" << std::endl;

// zeros
    std::map<int, int> exc_zeros =
      boost::assign::map_list_of
        (35, 1800) (34, 1900) (31, 2100)
        (10, 550)(11, 700)
	(17, 350);
    std::vector<int> zeros(ax12_ids.size());
    dynamixel::Status status;
    for (size_t i = 0; i < ax12_ids.size(); ++i)
      if (exc_zeros.find(ax12_ids[i]) != exc_zeros.end())
        zeros[i] = exc_zeros[ax12_ids[i]];
      else
      if (ax12_ids[i] <= 30)
        zeros[i] = 512;
      else
        zeros[i] = 2048;
// special cases


    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
      controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], true));
      controller.recv(READ_DURATION, status);
    }
    usleep(5e5);
    std::vector<int> pos = zeros;
    controller.send(ax12::SetPositions(ax12_ids, pos));


  }
  catch(dynamixel::Error e)
  {
    std::cerr << "error (dynamixel):" << e.msg() << std::endl;
  }

}
