/*
 * Appender.cpp
 *
 */

#include "Persistency/IO/Postgres/detail/Appender.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

Appender::Appender(Transaction &tr):
  tr_(tr)
{
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
