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

MetaAlert::MetaAlert(Persistency::MetaAlertPtrNN  ma,
                     const Transaction           &/*t*/):
  ma_(ma)
{
  assert( ma_.get()!=NULL );
}

MetaAlert::~MetaAlert(void)
{
}

void MetaAlert::save()
{
  saveImpl();
}

void MetaAlert::markAsUsed()
{
  markAsUsedImpl();
}

void MetaAlert::markAsUnused()
{
  markAsUnusedImpl();
}

void MetaAlert::updateSeverityDelta(double delta)
{
  updateSeverityDeltaImpl(delta);

  assert(ma_.get()!=NULL);
  ma_->updateSeverityDelta(delta);
}

void MetaAlert::updateCertanityDelta(double delta)
{
  updateCertanityDeltaImpl(delta);

  assert(ma_.get()!=NULL);
  ma_->updateCertanityDelta(delta);
}

void MetaAlert::addChild(Persistency::MetaAlertPtrNN child)
{
  addChildImpl(child);
}

void MetaAlert::associateWithAlert(Persistency::AlertPtrNN alert)
{
  associateWithAlert(alert);
}

const Persistency::MetaAlert &MetaAlert::get(void) const
{
  assert(ma_.get()!=NULL);
  return *ma_;
}

} // namespace IO
} // namespace Persistency
