/*
 * Host.cpp
 *
 */
#include "Persistency/IO/Host.hpp"

namespace Persistency
{
namespace IO
{

Host::Host(Persistency::HostPtrNN  host,
           Transaction            &t):
  host_(host),
  t_(t)
{
  t_.ensureIsActive();
}

Host::~Host(void)
{
}

void Host::setName(const Persistency::Host::Name &name)
{
  t_.ensureIsActive();
  // set name in memeory and then save it to persistent storage. this allows
  // to throw exception when trying to set already set name in the saving
  // implementaiton not the data base backend and so is more generic and
  // consistent with reast of the code.
  host_->setName(name);
  setNameImpl(t_, name);
}

Persistency::HostPtrNN Host::get(void)
{
  assert(host_.get()!=NULL);
  return host_;
}

} // namespace IO
} // namespace Persistency
