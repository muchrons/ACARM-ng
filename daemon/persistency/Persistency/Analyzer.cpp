/*
 * Analyzer.cpp
 *
 */

#include "Persistency/Analyzer.hpp"
#include "Base/ViaPointer.hpp"

namespace Persistency
{

Analyzer::Analyzer(const Name    &name,
                   const Version *version,
                   const OS      *os,
                   const IP      *ip):
  name_(name),
  version_(version),
  os_(os),
  ip_(ip)
{
}

const Analyzer::Name &Analyzer::getName(void) const
{
  return name_;
}

const Analyzer::Version *Analyzer::getVersion(void) const
{
  return version_.get();
}

const Analyzer::OS *Analyzer::getOS(void) const
{
  return os_.get();
}

const Analyzer::IP *Analyzer::getIP(void) const
{
  return ip_.get();
}

bool Analyzer::operator==(const Analyzer &other) const
{
  if(this==&other)
    return true;

  if( getName()!=other.getName() )
    return false;
  if( !Base::ViaPointer::equal( getVersion(), other.getVersion() ) )
    return false;
  if( !Base::ViaPointer::equal( getOS(), other.getOS() ) )
    return false;
  if( !Base::ViaPointer::equal( getIP(), other.getIP() ) )
    return false;
  return true;
}

} // namespace Persistency
