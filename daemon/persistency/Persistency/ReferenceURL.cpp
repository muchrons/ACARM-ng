/*
 * ReferenceURL.cpp
 *
 */

#include "Persistency/ReferenceURL.hpp"

namespace Persistency
{

ReferenceURL::ReferenceURL(const Name &name, const URL &url):
  name_(name),
  url_(url)
{
}

const ReferenceURL::Name &ReferenceURL::getName(void) const
{
  return name_;
}

const ReferenceURL::URL &ReferenceURL::getURL(void) const
{
  return url_;
}

bool ReferenceURL::operator==(const ReferenceURL &other) const
{
  if(this==&other)
    return true;

  if( getName()!=other.getName() )
    return false;
  if( getURL()!=other.getURL() )
    return false;
  return true;
}

} // namespace Persistency
