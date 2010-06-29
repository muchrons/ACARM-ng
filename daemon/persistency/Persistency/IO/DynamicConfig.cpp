/*
 * DynamicConfig.cpp
 *
 */
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{

DynamicConfig::DynamicConfig(const char *owner, Transaction &t):
  owner_(owner),
  t_(t)
{
}

DynamicConfig::~DynamicConfig(void)
{
}

void DynamicConfig::write(const std::string &key, const std::string &value)
{
  writeImpl(t_, key, value);
}

DynamicConfig::StringNULL DynamicConfig::read(const std::string &key)
{
  return readImpl(t_, key);
}

std::string DynamicConfig::readConst(const std::string &key)
{
  return readConstImpl(t_, key);
}

const char *DynamicConfig::getOwner(void) const
{
  if( owner_.get()==NULL )
    return NULL;
  return owner_->c_str();
}

} // namespace IO
} // namespace Persistency
