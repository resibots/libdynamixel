#include <iostream>
#include <dynamixel/dynamixel.hpp>
#include <cmath>

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

    std::vector<int> zeros(ax12_ids.size());
    dynamixel::Status status;

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
      controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], true));
      controller.recv(READ_DURATION, status);
    }
    usleep(2.5 * 1e5);

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
      controller.send(ax12::GetPosition(ax12_ids[i]));
      controller.recv(READ_DURATION, status);
      std::cout << "status:" << status << std::endl;
      zeros[i] = 512; //status.decode16();
      std::cout << (int)ax12_ids[i] << "->" << status.decode16() << std::endl;
    }



    float x = 0;
    std::vector<int> pos(ax12_ids.size());
    while (1)
    {
      size_t k = 0;
      for (size_t i = 0; i < ax12_ids.size(); ++i)
      {
        if (ax12_ids[i] != 12 && ax12_ids[i] != 13)
          pos[i] = (int)(sin(x + (k++) * 2 ) * 120 + zeros[i]);
        else
          pos[i] = 512;//sin(x + k *2)*150 + zeros[i];//400;
        std::cout << (int)ax12_ids[i] << "=>" << pos[i] << std::endl;
      }
      std::cout << "----" << std::endl;
      dynamixel::ax12::SetPositions set_positions(ax12_ids, pos);
      controller.send(set_positions);
      //controller.recv(READ_DURATION, status);

      x += 0.025;
    }

  }
  catch(dynamixel::Error e)
  {
    std::cerr << "error (dynamixel):" << e.msg() << std::endl;
  }

}
