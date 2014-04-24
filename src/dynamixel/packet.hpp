#ifndef DYNAMIXEL_PACKET_HPP_
#define DYNAMIXEL_PACKET_HPP_

#include <vector>
#include <cassert>
#include <cstdlib>
#include "misc.hpp"

namespace dynamixel
{
  byte_t compute_checksum(const byte_t* packet,
			  size_t packet_size);
}
#endif
