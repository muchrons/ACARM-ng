/*
 * Message.cpp
 *
 */
#include <cstring>

#include "Trigger/SnortSam/Ver14/Message.hpp"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

Message::Message(void)
{
  memset(&p_, 0, sizeof(p_));
  p_.endianCheck_=1;        // will be 0x0001 (big endian) or 0x0100 (little endian)
}

Message::Message(const SamPacket &p):
  p_(p)
{
}

void Message::setIP(uint8_t (&out)[4], const IPv4 &ip)
{
  const IPv4::bytes_type &b=ip.to_bytes();
  for(int i=0; i<4; ++i)
    out[i]=b[i];
}

Message::IPv4 Message::getIP(const uint8_t (&in)[4])
{
  const IPv4::bytes_type tmp={{in[0], in[1], in[2], in[3]}};
  return IPv4(tmp);
}

void Message::setNum(uint8_t (&out)[2], const uint16_t in)
{
  out[0]=(in>>0)&0x00FF;
  out[1]=(in>>8)&0x00FF;
}

uint16_t Message::getNum(const uint8_t (&in)[2])
{
  return (in[1]<<8)|(in[0]<<0);
}

void Message::setNum(uint8_t (&out)[4], const uint32_t in)
{
  out[0]=(in>> 0)&0x000000FF;
  out[1]=(in>> 8)&0x000000FF;
  out[2]=(in>>16)&0x000000FF;
  out[3]=(in>>24)&0x000000FF;
}

uint32_t Message::getNum(const uint8_t (&in)[4])
{
  return (in[3]<<24)|(in[2]<<16)|(in[1]<< 8)|(in[0]<< 0);
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
