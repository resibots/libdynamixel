#include "packet.hpp"

namespace dynamixel
{
  byte_t compute_checksum(const byte_t* packet,
			  size_t packet_size)
  {
    assert(packet);
    assert(packet_size);
    assert(packet[0] == 0xFF);
    assert(packet[1] == 0xFF);
    int sum = 0;
    for (size_t i = 2; i < packet_size - 1; ++i)
      sum += packet[i];
    byte_t checksum = (sum & 0xFF);
    assert((sum > 255) || (sum == checksum));
    return ~checksum;
  } 
}
