/*
 * Analyzer.cpp
 *
 */

#include "Persistency/Analyzer.hpp"


namespace Persistency
{

const Analyzer::Name &Analyzer::getName(void) const
{
  return name_;
}

const Host *Analyzer::getHost(void) const
{
  return host_.get();
}

Analyzer::Analyzer(const Name &name,
                   HostPtr     host):
  name_(name),
  host_(host)
{
}

} // namespace Persistency
