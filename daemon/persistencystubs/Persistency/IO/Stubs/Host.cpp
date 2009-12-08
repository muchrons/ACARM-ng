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
           const Transaction    &t):
  IO::Host(host, t)
{
}

void Host::setNameImpl(const Persistency::Host::Name &/*name*/)
{
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
