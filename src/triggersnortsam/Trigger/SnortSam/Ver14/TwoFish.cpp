/*
 * TwoFish.cpp
 *
 */
#include "Trigger/SnortSam/Ver14/TwoFish.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

struct TwoFish::PImpl
{
  explicit PImpl(const std::string &key):
    key_(key)
  {
  }

  const std::string &key_;
};



TwoFish::TwoFish(const std::string &key):
  impl_(new PImpl(key))
{
}

TwoFish::~TwoFish(void)
{
  // ensure safe destruction of PImpl
}

void TwoFish::encryptImpl(const uint8_t *data, size_t len)
{
  // TODO
}

void TwoFish::decryptImpl(const uint8_t *data, size_t len)
{
  // TODO
}

TwoFish::DataRef TwoFish::getDataImpl(void)
{
  // TODO
  const uint8_t *ptr=reinterpret_cast<const uint8_t*>("");
  return DataRef(ptr, 0);
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
