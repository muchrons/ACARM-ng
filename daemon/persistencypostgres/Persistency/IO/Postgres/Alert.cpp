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
             Transaction             &t,
             DBHandlerPtrNN           dbHandler):
  IO::Alert(alert, t),
  dbHandler_(dbHandler)
{
}

void Alert::saveImpl(Transaction &)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
