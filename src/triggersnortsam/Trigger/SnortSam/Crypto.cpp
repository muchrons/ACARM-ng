/*
 * Crypto.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Trigger/SnortSam/Crypto.hpp"

namespace Trigger
{
namespace SnortSam
{

Crypto::Crypto(void):
  log_("trigger.snortsam.crypto")
{
  LOGMSG_DEBUG(log_, "initializing cryptographic interface");
}

Crypto::~Crypto(void)
{
  LOGMSG_DEBUG(log_, "destroying cryptographic interface");
}

DataRef Crypto::encrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  LOGMSG_DEBUG_S(log_)<<"encrypting "<<len<<" bytes od data";
  encryptImpl(data, len);
  return getData();
}

DataRef Crypto::decrypt(const uint8_t *data, size_t len)
{
  if(data==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "NULL data buffer given");
  LOGMSG_DEBUG_S(log_)<<"decrypting "<<len<<" bytes od data";
  decryptImpl(data, len);
  return getData();
}

DataRef Crypto::getData(void) const
{
  return getDataImpl();
}

} // namespace SnortSam
} // namespace Trigger
