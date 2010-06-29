/*
 * DynamicConfig.t.cpp
 *
 */
#include "Persistency/IO/Stubs/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

DynamicConfig::DynamicConfig(const char *owner, Persistency::IO::Transaction &t):
  Persistency::IO::DynamicConfig(owner, t),
  callsWrite_(0),
  callsRead_(0),
  callsReadConst_(0)
{
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &/*t*/, const std::string &/*key*/, const std::string &/*value*/)
{
  ++callsWrite_;
}

DynamicConfig::StringNULL DynamicConfig::readImpl(Persistency::IO::Transaction &/*t*/, const std::string &/*key*/)
{
  ++callsRead_;
  return StringNULL("alice has a wonderland");
}

std::string DynamicConfig::readConstImpl(Persistency::IO::Transaction &/*t*/, const std::string &/*key*/)
{
  ++callsReadConst_;
  return "i'm const";
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
