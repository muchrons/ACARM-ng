/*
 * MetaAlert.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{

MetaAlert::MetaAlert(Persistency::MetaAlertPtr  ma,
                     const Transaction         &/*t*/):
  ma_(ma)
{
  if( ma_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "ma");
}

MetaAlert::~MetaAlert(void)
{
}

Persistency::MetaAlert &MetaAlert::get(void)
{
  assert(ma_.get()!=NULL);
  return *ma_;
}

} // namespace IO
} // namespace Persistency
