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
  // encryption
  TwoFish::DataRef enc=tf_.encrypt(buf, sizeof(buf));
  ensure("output buffer is too small", enc.len_>=sizeof(buf));
  // decryption
  TwoFish          tmp("abc");
  TwoFish::DataRef dec=tmp.decrypt(enc.data_, enc.len_);
  // check
  ensure_equals("invalid data length", dec.len_, sizeof(buf));
  ensure("got NULL pointer", dec.data_!=NULL);
  for(size_t i=0; i<dec.len_; ++i)
    ensure_equals("invalid byte decoded", dec.data_[i], buf[i]);
}

// test decrypting some random data (w/o prior encryption)
template<>
template<>
void testObj::test<3>(void)
{
  const uint8_t    buf[]={1,2,3,4,5,6};
  try
  {
    tf_.decrypt(buf, sizeof(buf));
    fail("decrypt() didn't throw on invalid data");
  }
  catch(const Trigger::SnortSam::ExceptionCryptoFailed&)
  {
    // this is expected
  }
}

} // namespace tut
