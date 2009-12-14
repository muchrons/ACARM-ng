/*
 * Host.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Stubs/Host.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

Host::Host(Persistency::HostPtr  host,
           const Transaction    &t,
           int                   handler):
  IO::Host(host, t),
  setNameCalls_(0)
{
  assert(handler==42);
  handler=0;
}

void Host::setNameImpl(const Persistency::Host::Name &/*name*/)
{
  ++setNameCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
