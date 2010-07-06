/*
 * Analyzer.cpp
 *
 */

#include "Persistency/Analyzer.hpp"
#include "Base/ViaPointer.hpp"

namespace Persistency
{

Analyzer::Analyzer(const ID               id,
                   const Name            &name,
                   const Version         &version,
                   const OperatingSystem &os,
                   const IP              *ip):
  id_(id),
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

const Analyzer::Version &Analyzer::getVersion(void) const
{
  return version_;
}

const Analyzer::OperatingSystem &Analyzer::getOperatingSystem(void) const
{
  return os_;
}

const Analyzer::IP *Analyzer::getIP(void) const
{
  return ip_.get();
}

Analyzer::ID Analyzer::getID(void) const
{
  return id_;
}

bool Analyzer::operator==(const Analyzer &other) const
{
  if(this==&other)
    return true;

  if( getID()!=other.getID() )
    return false;
  if( getName()!=other.getName() )
    return false;
  if( getVersion()!=other.getVersion() )
    return false;
  if( getOperatingSystem()!=other.getOperatingSystem() )
    return false;
  if( !Base::ViaPointer::equal( getIP(), other.getIP() ) )
    return false;
  return true;
}

} // namespace Persistency
