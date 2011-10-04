/*
 * TwoFish.t.cpp
 *
 */
#include <tut.h>
#include <utility>

#include "Trigger/SnortSam/Ver14/TwoFish.hpp"

using Trigger::SnortSam::DataRef;
using namespace Trigger::SnortSam::Ver14;

namespace
{

struct TestClass
{
  TestClass(void):
    e_("abc"),
    d_("abc")
  {
  }

  TwoFish e_;
  TwoFish d_;
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
  const uint8_t buf[]={1,2,3,4,5,6};
  DataRef       out  =e_.encrypt(buf, sizeof(buf));
  ensure("output buffer is too small", out.size()>=sizeof(buf));
}

// try encrypting and decrypting some portion of data and see if it gives the same result
template<>
template<>
void testObj::test<2>(void)
{
  const uint8_t buf[]={1,2,3,4,5,6};
  // encryption
  DataRef enc=e_.encrypt(buf, sizeof(buf));
  ensure("output buffer is too small", enc.size()>=sizeof(buf));
  // decryption
  DataRef dec=d_.decrypt(enc.data(), enc.size());
  // check
  ensure_equals("invalid data length", dec.size(), sizeof(buf));
  ensure("got NULL pointer", dec.data()!=NULL);
  for(size_t i=0; i<dec.size(); ++i)
    ensure_equals("invalid byte decoded", dec[i], buf[i]);
}

// test decrypting some random data (w/o prior encryption)
template<>
template<>
void testObj::test<3>(void)
{
  const uint8_t    buf[]={1,2,3,4,5,6};
  try
  {
    d_.decrypt(buf, sizeof(buf));
    fail("decrypt() didn't throw on invalid data");
  }
  catch(const Trigger::SnortSam::ExceptionCryptoFailed&)
  {
    // this is expected
  }
}

// try enc/dec few times
template<>
template<>
void testObj::test<4>(void)
{
  uint8_t buf[]={1,2,3,4,5,6};
  TwoFish *a=&e_;
  TwoFish *b=&d_;
  for(int i=0; i<5; ++i)
  {
    // make some random changes
    buf[4]=42-i;
    buf[2]=42+i;
    // encryption
    DataRef enc=e_.encrypt(buf, sizeof(buf));
    ensure("output buffer is too small", enc.size()>=sizeof(buf));
    // decryption
    DataRef dec=d_.decrypt(enc.data(), enc.size());
    // check
    ensure_equals("invalid data length", dec.size(), sizeof(buf));
    ensure("got NULL pointer", dec.data()!=NULL);
    for(size_t i=0; i<dec.size(); ++i)
      ensure_equals("invalid byte decoded", dec[i], buf[i]);
    // use encryption part for decryption and vice versa
    std::swap(a, b);
  }
}

} // namespace tut
