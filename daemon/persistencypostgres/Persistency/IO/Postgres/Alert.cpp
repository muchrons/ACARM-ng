/*
 * Alert.hpp
 *
 */
#include "Persistency/IO/Postgres/Alert.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Alert::Alert(Persistency::AlertPtrNN  alert,
             const Transaction       &t,
             DBHandlerPtrNN           dbHandler):
  IO::Alert(alert, t),
  dbHandler_(dbHandler)
{
}

void Alert::saveImpl(void)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
