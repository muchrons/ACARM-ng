/*
 * NoCrypto.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/NoCrypto.hpp"

using namespace Trigger::SnortSam;

namespace
{

struct TestClass
{
  NoCrypto nc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/NoCrypto");
} // unnamed namespace


namespace tut
{

// test "encrypting"
template<>
template<>
void testObj::test<1>(void)
{
  const uint8_t           buf[]={4,2};
  const NoCrypto::DataRef out  =nc_.encrypt(buf, sizeof(buf));
  ensure_equals("invalid size", out.len_, 2u);
  ensure("NULL pointer returned", out.data_!=NULL);
  ensure_equals("invalid element 0", out.data_[0], 4);
  ensure_equals("invalid element 1", out.data_[1], 2);
}

// test encrypting empty data set
template<>
template<>
void testObj::test<2>(void)
{
  const uint8_t           buf[]={4,2};
  const NoCrypto::DataRef out  =nc_.encrypt(buf, 0);
  ensure_equals("invalid size", out.len_, 0u);
  ensure("NULL pointer returned", out.data_!=NULL);
}

// test "decryption"
template<>
template<>
void testObj::test<3>(void)
{
  const uint8_t           buf[]={4,2};
  const NoCrypto::DataRef out  =nc_.decrypt(buf, sizeof(buf));
  ensure_equals("invalid size", out.len_, 2u);
  ensure("NULL pointer returned", out.data_!=NULL);
  ensure_equals("invalid element 0", out.data_[0], 4);
  ensure_equals("invalid element 1", out.data_[1], 2);
}

// test buffor resizing
template<>
template<>
void testObj::test<4>(void)
{
  const uint8_t *ptr=NULL;
  // some initial size
  {
    const uint8_t           buf[]={4,2};
    const NoCrypto::DataRef out  =nc_.decrypt(buf, sizeof(buf));
    ensure_equals("invalid size", out.len_, 2u);
    ensure("NULL pointer returned", out.data_!=NULL);
    ptr=out.data_;
  }
  // resize will take place here
  {
    const uint8_t           buf[]={4,2,6,6,6};
    const NoCrypto::DataRef out  =nc_.decrypt(buf, sizeof(buf));
    ensure_equals("invalid size", out.len_, 5u);
    ensure("NULL pointer returned", out.data_!=NULL);
    ensure("pointer ha NOT been reallocated", ptr!=out.data_);
  }
}

// test no buffor resizing, when smaller type requrested first
template<>
template<>
void testObj::test<5>(void)
{
  const uint8_t *ptr=NULL;
  // some initial size
  {
    const uint8_t           buf[]={4,2,6,6,6};
    const NoCrypto::DataRef out  =nc_.decrypt(buf, sizeof(buf));
    ensure_equals("invalid size", out.len_, 5u);
    ensure("NULL pointer returned", out.data_!=NULL);
    ptr=out.data_;
  }
  // no resize now
  {
    const uint8_t           buf[]={4,2};
    const NoCrypto::DataRef out  =nc_.decrypt(buf, sizeof(buf));
    ensure_equals("invalid size", out.len_, 2u);
    ensure("NULL pointer returned", out.data_!=NULL);
    ensure("pointer ha been reallocated", ptr==out.data_);
  }
}

} // namespace tut
