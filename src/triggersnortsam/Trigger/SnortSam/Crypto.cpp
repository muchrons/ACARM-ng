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

Crypto::DataRef Crypto::encrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  encryptImpl(data, len);
  return getData();
}

Crypto::DataRef Crypto::decrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  decryptImpl(data, len);
  return getData();
}

Crypto::DataRef Crypto::getData(void) const
{
  const DataRef tmp=getDataImpl();
  assert(tmp.data_!=NULL);
  return tmp;
}

} // namespace SnortSam
} // namespace Trigger
