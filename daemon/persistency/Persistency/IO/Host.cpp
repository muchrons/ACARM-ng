/*
 * Host.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Host.hpp"

namespace Persistency
{
namespace IO
{

Host::Host(Persistency::HostPtr  host,
           const Transaction    &/*t*/):
  host_(host)
{
  if( host_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "host");
}

Host::~Host(void)
{
}

void Host::setName(const Persistency::Host::Name &name)
{
  setNameImpl(name);

  assert(host_.get()!=NULL);
  host_->setName(name);
}

const Persistency::Host &Host::get(void) const
{
  assert(host_.get()!=NULL);
  return *host_;
}

} // namespace IO
} // namespace Persistency
