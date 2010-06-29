/*
 * DynamicConfig.cpp
 *
 */
#include "Persistency/IO/Stubs/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

DynamicConfig::DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t, int /*handle*/):
  Persistency::IO::DynamicConfig(owner, t),
  callsWrite_(0),
  callsRead_(0),
  callsReadConst_(0)
{
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/, const Value &/*value*/)
{
  ++callsWrite_;
}

DynamicConfig::ValueNULL DynamicConfig::readImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  ++callsRead_;
  return ValueNULL( Value("alice has a wonderland") );
}

DynamicConfig::Value DynamicConfig::readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  ++callsReadConst_;
  return "i'm const";
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
