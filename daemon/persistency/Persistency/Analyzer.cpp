/*
 * Analyzer.cpp
 *
 */

#include "Persistency/Analyzer.hpp"
#include "Base/ViaPointer.hpp"

namespace Persistency
{

Analyzer::Analyzer(const Name &name,
                   HostPtr     host):
  name_(name),
  host_(host)
{
}

const Analyzer::Name &Analyzer::getName(void) const
{
  return name_;
}

HostPtr Analyzer::getHost(void) const
{
  return host_;
}

bool Analyzer::operator==(const Analyzer &other) const
{
  if( getName()!=other.getName() )
    return false;
  if( !Base::ViaPointer::equal( getHost().get(), other.getHost().get() ) )
    return false;
  return true;
}

} // namespace Persistency
