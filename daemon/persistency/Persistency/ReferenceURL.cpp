/*
 * ReferenceURL.cpp
 *
 */

#include "Persistency/ReferenceURL.hpp"

namespace Persistency
{

const ReferenceURL::Name &ReferenceURL::getName(void) const
{
  return name_;
}

const ReferenceURL::URL &ReferenceURL::getURL(void) const
{
  return url_;
}

ReferenceURL::ReferenceURL(const Name &name, const URL &url):
  name_(name),
  url_(url)
{
}

} // namespace Persistency
