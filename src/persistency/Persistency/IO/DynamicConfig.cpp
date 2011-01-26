/*
 * DynamicConfig.cpp
 *
 */
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{

DynamicConfig::IterationCallback::~IterationCallback(void)
{
}

DynamicConfig::DynamicConfig(const Owner &owner, Transaction &t):
  owner_(owner),
  t_(t)
{
}

DynamicConfig::~DynamicConfig(void)
{
}

void DynamicConfig::write(const Key &key, const Value &value)
{
  writeImpl(t_, key, value);
}

DynamicConfig::ValueNULL DynamicConfig::read(const Key &key)
{
  return readImpl(t_, key);
}

DynamicConfig::Value DynamicConfig::readConst(const Key &key)
{
  return readConstImpl(t_, key);
}

void DynamicConfig::remove(const Key &key)
{
  removeImpl(t_, key);
}

void DynamicConfig::iterate(IterationCallback &cb)
{
  iterateImpl(t_, cb);
}

const DynamicConfig::Owner &DynamicConfig::getOwner(void) const
{
  return owner_;
}

} // namespace IO
} // namespace Persistency
