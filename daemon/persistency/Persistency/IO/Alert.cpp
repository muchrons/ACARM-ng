/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/TryCatchInAPI.hpp"

namespace Persistency
{
namespace IO
{

Alert::Alert(Persistency::AlertPtrNN  alert,
             Transaction             &t):
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
  TRYCATCH_BEGIN
    t_.ensureIsActive();
    saveImpl(t_);
  TRYCATCH_END
}

Persistency::AlertPtrNN Alert::get(void)
{
  assert(alert_.get()!=NULL);
  return alert_;
}

} // namespace IO
} // namespace Persistency
