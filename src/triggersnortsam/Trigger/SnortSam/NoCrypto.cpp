/*
 * NoCrypto.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "Trigger/SnortSam/NoCrypto.hpp"


namespace Trigger
{
namespace SnortSam
{

NoCrypto::NoCrypto(void):
  buf_(new uint8_t[0]),     // buffer cannot be NULL at any time
  size_(0),
  used_(0)
{
  assert(buf_.get()!=NULL);
}

void NoCrypto::encryptImpl(const uint8_t *data, size_t len)
{
  resize(len);
  memcpy(buf_.get(), data, len);
}

void NoCrypto::decryptImpl(const uint8_t *data, size_t len)
{
  resize(len);
  memcpy(buf_.get(), data, len);
}

NoCrypto::DataRef NoCrypto::getDataImpl(void) const
{
  return DataRef(buf_.get(), used_);
}

void NoCrypto::resize(const size_t len)
{
  if(len>size_)
  {
    // needs new, bigger buffer
    boost::scoped_array<uint8_t> tmp(new uint8_t[len]);
    buf_.swap(tmp);
    size_=len;
  }
  // mark how big buffer has been requested right this time
  used_=len;
}

} // namespace SnortSam
} // namespace Trigger
