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

Host::Host(Persistency::HostPtrNN  host,
           Transaction            &t,
           int                     /*handler*/):
  IO::Host(host, t),
  setNameCalls_(0)
{
}

void Host::setNameImpl(Transaction &, const Persistency::Host::Name &/*name*/)
{
  ++setNameCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
