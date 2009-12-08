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

Alert::Alert(Persistency::AlertPtr  alert,
             const Transaction     &t,
             int                    handler):
  IO::Alert(alert, t)
{
  assert(handler==42);
}

void Alert::saveImpl(const Persistency::Alert &/*alert*/)
{
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
