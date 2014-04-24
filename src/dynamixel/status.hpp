#ifndef _DYNAMIXEL_STATUS_HPP_
#define _DYNAMIXEL_STATUS_HPP_

#include "error.hpp"
#include "packet.hpp"

namespace dynamixel
{
  class Status
  {
    public:
      Status() : _id(-1), _length(-1) {
      }

      Status(const byte_t * packet, int packet_size)
      {
        if (!decode_packet(packet, packet_size))
          throw Error("Status: bad packet in constructor");
      }

       // return false if the packet is not complete
      bool decode_packet(const byte_t*packet, int packet_size);
      std::ostream& print(std::ostream& os) const;
      byte_t get_id() { return _id; }
      byte_t get_length() { return _length; }
      std::vector<byte_t> get_params() { return _params; }
       // decode 2 bytes of params as a 2 bytes integer
      int decode16() { return (((int)_params[1]) << 8) | _params[0]; }
    protected:
      int _id;
      int _length;
      std::vector<byte_t> _params;
  };

  inline std::ostream& operator << (std::ostream & os, const Status &st)
  {
    return st.print(os);
  }
}
#endif
