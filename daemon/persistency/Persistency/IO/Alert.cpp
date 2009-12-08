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

Alert::Alert(Persistency::AlertPtr  alert,
             const Transaction     &/*t*/):
  alert_(alert)
{
  if( alert_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "alert");
}

Alert::~Alert(void)
{
}

Persistency::Alert &Alert::get(void)
{
  assert(alert_.get()!=NULL);
  return *alert_;
}

} // namespace IO
} // namespace Persistency
