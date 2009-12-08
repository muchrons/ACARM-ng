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

Persistency::Host &Host::get(void)
{
  assert(host_.get()!=NULL);
  return *host_;
}

} // namespace IO
} // namespace Persistency
