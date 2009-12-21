/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Alert.hpp"

namespace Persistency
{
namespace IO
{

Alert::Alert(Persistency::AlertPtrNN  alert,
             const Transaction       &t):
  alert_(alert),
  t_(t)
{
  t_.ensureIsActive();
}

Alert::~Alert(void)
{
}

void Alert::save(void)
{
  t_.ensureIsActive();
  saveImpl();
}

const Persistency::Alert &Alert::get(void) const
{
  assert(alert_.get()!=NULL);
  return *alert_;
}

} // namespace IO
} // namespace Persistency
