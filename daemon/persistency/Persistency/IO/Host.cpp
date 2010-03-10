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
  setNameImpl(t_, name);

  assert(host_.get()!=NULL);
  host_->setName(name);
}

Persistency::HostPtrNN Host::get(void)
{
  assert(host_.get()!=NULL);
  return host_;
}

} // namespace IO
} // namespace Persistency
