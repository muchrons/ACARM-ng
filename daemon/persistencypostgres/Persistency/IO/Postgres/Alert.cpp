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
             DBSharedConnection       sc):
  IO::Alert(alert, t),
  sc_(sc)
{
}

void Alert::saveImpl(void)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
