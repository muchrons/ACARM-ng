/*
 * Message.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/Ver14/Message.hpp"

using namespace Trigger::SnortSam::Ver14;

namespace
{

struct TestClass
{
  Message m_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Ver14/Message");
} // unnamed namespace


namespace tut
{

// check initial zeroing
template<>
template<>
void testObj::test<1>(void)
{
  const uint8_t sizeOfEndianCheck=2;
  for(const uint8_t *b=m_.begin()+sizeOfEndianCheck; b!=m_.end(); ++b)
    ensure_equals("byte not zeroed", *b, 0);
}

// check size - must be exact in order to work...
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid size of the struct - protocol will not work...", m_.size(), 32u);
}

// check if endian check is set
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid endian check field", m_.p_.endianCheck_, 1u);
}

// test setting some IP
template<>
template<>
void testObj::test<4>(void)
{
  uint8_t ip[4];
  m_.setIP( ip, Message::IPv4::from_string("10.20.30.40") );
  for(uint8_t i=0; i<4; ++i)
    ensure_equals("invalid elements set", ip[i], (i+1)*10);
}

// test setting some 16-bit number
template<>
template<>
void testObj::test<5>(void)
{
  uint8_t        num[2];
  const uint16_t n=0x0201;
  m_.setNum(num, n);
  for(uint8_t i=0; i<2; ++i)
    ensure_equals("invalid elements set", static_cast<int>(num[i]), (i+1));
}

// test setting some 32-bit number
template<>
template<>
void testObj::test<6>(void)
{
  uint8_t        num[4];
  const uint32_t n=0x04030201;
  m_.setNum(num, n);
  for(uint8_t i=0; i<4; ++i)
    ensure_equals("invalid elements set", static_cast<int>(num[i]), (i+1));
}

// test iterators
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("invalid elements count", m_.end()-m_.begin(), m_.size());
}

// try building from packet
template<>
template<>
void testObj::test<8>(void)
{
  const SamPacket p={42, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 0,0,0, {0,0,0,0}, 0};
  const Message   m(p);
  ensure_equals("invalid value", m.p_.endianCheck_, 42);
}

// test getting IP from field
template<>
template<>
void testObj::test<9>(void)
{
  uint8_t             ip[4]={1,2,3,4};
  const Message::IPv4 out  =m_.getIP(ip);
  ensure_equals("invalid IP", out.to_string(), "1.2.3.4");
}

// test getting 16-bit int from fields
template<>
template<>
void testObj::test<10>(void)
{
  uint8_t        num[2]={0x01, 0x02};
  const uint16_t out   =m_.getNum(num);
  ensure_equals("invalid 16-bit number", out, 0x0201);
}

// test getting 32-bit int from fields
template<>
template<>
void testObj::test<11>(void)
{
  uint8_t        num[4]={0x01, 0x02, 0x03, 0x04};
  const uint32_t out   =m_.getNum(num);
  ensure_equals("invalid 32-bit number", out, 0x04030201);
}

// test setting and getting 16-bit int
template<>
template<>
void testObj::test<12>(void)
{
  const uint16_t in=0x1234;
  uint8_t        buf[2];
  m_.setNum(buf, in);
  const uint16_t out=m_.getNum(buf);
  ensure_equals("invalid 16-bit number", out, in);
}

// test setting and getting 32-bit int
template<>
template<>
void testObj::test<13>(void)
{
  const uint32_t in=0x12345678;
  uint8_t        buf[4];
  m_.setNum(buf, in);
  const uint32_t out=m_.getNum(buf);
  ensure_equals("invalid 32-bit number", out, in);
}

// test getting and setting IP
template<>
template<>
void testObj::test<14>(void)
{
  const Message::IPv4 in=Message::IPv4::from_string("2.3.4.5");
  uint8_t             buf[4];
  m_.setIP(buf, in);
  const Message::IPv4 out=m_.getIP(buf);
  ensure_equals("invalid IP", out.to_string(), in.to_string());
}

} // namespace tut
