/*
 * TwoFish.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/Ver14/TwoFish.hpp"

using namespace Trigger::SnortSam::Ver14;

namespace
{

struct TestClass
{
  TestClass(void):
    tf_("abc")
  {
  }

  TwoFish tf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Ver14/TwoFish");
} // unnamed namespace


namespace tut
{

// test encrypting some random data
template<>
template<>
void testObj::test<1>(void)
{
  const uint8_t    buf[]={1,2,3,4,5,6};
  TwoFish::DataRef out  =tf_.encrypt(buf, sizeof(buf));
  ensure("output buffer is too small", out.len_>=sizeof(buf));
}

// try encrypting and decrypting some portion of data and see if it gives the same result
template<>
template<>
void testObj::test<2>(void)
{
  const uint8_t    buf[]={1,2,3,4,5,6};
  TwoFish::DataRef enc  =tf_.encrypt(buf, sizeof(buf));
  TwoFish          tmp("abc");
  TwoFish::DataRef dec  =tf_.encrypt(enc.data_, enc.len_);
  ensure_equals("invalid data length", dec.len_, sizeof(buf));
  ensure("got NULL pointer", enc.data_!=NULL);
  for(size_t i=0; i<dec.len_; ++i)
    ensure_equals("invalid byte decoded", dec.data_[i], buf[i]);
}

} // namespace tut
