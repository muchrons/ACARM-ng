/*
 * Crypto.t.cpp
 *
 */
#include <tut.h>
#include <boost/scoped_array.hpp>

#include "Trigger/SnortSam/Crypto.hpp"

using namespace Trigger::SnortSam;

namespace
{

bool g_dtorCalled;

struct TestCrypto: public Crypto
{
  TestCrypto(void):
    key_(13),
    len_(0)
  {
    g_dtorCalled=false;
  }
  ~TestCrypto(void)
  {
    g_dtorCalled=true;
  }

private:
  virtual void encryptImpl(const uint8_t *data, size_t len)
  {
    resize(len+1);
    for(size_t i=0; i<len; ++i)
      buf_[i]=(data[i]+key_)%256;
    buf_[len]=42;
  }

  virtual void decryptImpl(const uint8_t *data, size_t len)
  {
    if(len==0)
    {
      resize(0);
      return;
    }
    resize(len-1);
    for(size_t i=0; i<len-1; ++i)
      buf_[i]=(data[i]-key_)%256;
    tut::ensure_equals("invalid magic byte", static_cast<int>(data[len-1]), 42u);
  }

  virtual DataRef getDataImpl(void) const
  {
    return DataRef( buf_.get(), len_ );
  }

  void resize(const size_t len)
  {
    boost::scoped_array<uint8_t> buf(new uint8_t[len]);
    buf_.swap(buf);
    len_=len;
  }

  uint8_t                      key_;
  boost::scoped_array<uint8_t> buf_;
  size_t                       len_;
};

struct TestClass
{
  TestCrypto tc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Crypto");
} // unnamed namespace


namespace tut
{

// test c-tor/dtor
template<>
template<>
void testObj::test<1>(void)
{
  {
    TestCrypto tc;
    ensure("pre-condition failed", g_dtorCalled==false);
  }
  ensure("d-tor is not polymorphic", g_dtorCalled==true);
}

// test exception when got NULL buffer
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    tc_.encrypt(NULL, 0);
    fail("encrypt() didn't throw on NULL");
  }
  catch(const ExceptionCryptoFailed&)
  {
    // this is expected
  }
}

// encryption didn't failed on NULL buffer
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    tc_.decrypt(NULL, 0);
    fail("decrypt() didn't throw on NULL");
  }
  catch(const ExceptionCryptoFailed&)
  {
    // this is expected
  }
}

// try encrypting some data
template<>
template<>
void testObj::test<4>(void)
{
  const uint8_t buf[]={10, 42};
  const DataRef tmp  =tc_.encrypt(buf, sizeof(buf));
  ensure_equals("invalid size", tmp.size(), 3);
  ensure("ogt empty buffer", tmp.data()!=NULL);
  ensure_equals("invalid byte 0", tmp[0], 23u);
  ensure_equals("invalid byte 1", tmp[1], 55u);
  ensure_equals("invalid byte 2", tmp[2], 42u);
}

// try decrypting some data
template<>
template<>
void testObj::test<5>(void)
{
  const uint8_t buf[]={36, 66, 42};
  const DataRef tmp  =tc_.decrypt(buf, sizeof(buf));
  ensure_equals("invalid size", tmp.size(), 2);
  ensure("ogt empty buffer", tmp.data()!=NULL);
  ensure_equals("invalid byte 0", tmp[0], 23u);
  ensure_equals("invalid byte 1", tmp[1], 53u);
}

// try encrypting 0 bytes
template<>
template<>
void testObj::test<6>(void)
{
  const uint8_t buf[]={0};
  const DataRef tmp  =tc_.encrypt(buf, 0);
  ensure_equals("invalid size", tmp.size(), 1);
  ensure("ogt empty buffer", tmp.data()!=NULL);
  ensure_equals("invalid byte 0", tmp[0], 42u);
}

// try decrypting zero bytes
template<>
template<>
void testObj::test<7>(void)
{
  const uint8_t buf[]={0};
  const DataRef tmp  =tc_.decrypt(buf, 0);
  ensure_equals("invalid size", tmp.size(), 0);
  ensure("ogt empty buffer", tmp.data()!=NULL);
}

} // namespace tut
