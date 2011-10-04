/*
 * Crypto.cpp
 *
 */
#include <cassert>

#include "Trigger/SnortSam/Crypto.hpp"

namespace Trigger
{
namespace SnortSam
{

Crypto::~Crypto(void)
{
}

DataRef Crypto::encrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  encryptImpl(data, len);
  return getData();
}

DataRef Crypto::decrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  decryptImpl(data, len);
  return getData();
}

DataRef Crypto::getData(void) const
{
  return getDataImpl();
}

} // namespace SnortSam
} // namespace Trigger
