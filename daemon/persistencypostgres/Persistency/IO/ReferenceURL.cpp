/*
 * ReferenceURL.cpp
 *
 */

#include "Persistency/Postgres/ReferenceURL.hpp"

namespace Persistency
{
namespace Postgres
{

ReferenceURL::ReferenceURL(const Name &name, const URL &url):
  Persistency::ReferenceURL(name, url)
{
  // TODO DB I/O
}

} // namespace Postgres
} // namespace Persistency
