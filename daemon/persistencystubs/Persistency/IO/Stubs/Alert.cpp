/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Stubs/Alert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

Alert::Alert(Persistency::AlertPtrNN  alert,
             Transaction             &t,
             int                      handler):
  IO::Alert(alert, t),
  saveCalls_(0)
{
  assert(handler==42);
  handler=0;
}

void Alert::saveImpl(Transaction &)
{
  ++saveCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
