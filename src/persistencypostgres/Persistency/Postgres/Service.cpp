/*
 * Service.cpp
 *
 */

#include "Persistency/Postgres/Service.hpp"

namespace Persistency
{
namespace Postgres
{

Service::Service(const Name      &name,
                 const Port      &port,
                 const Protocol  &protocol,
                 ReferenceURLPtr  url):
  Persistency::Service(name, port, protocol, url)
{
  // TODO DB I/O
}

} // namespace Postgres
} // namespace Persistency
