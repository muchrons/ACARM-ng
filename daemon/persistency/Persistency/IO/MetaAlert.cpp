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
                     Transaction                 &t):
  ma_(ma),
  t_(t)
{
  assert( ma_.get()!=NULL );
  t_.ensureIsActive();
}

MetaAlert::~MetaAlert(void)
{
}

void MetaAlert::save()
{
  t_.ensureIsActive();
  saveImpl();
}

void MetaAlert::markAsUsed()
{
  t_.ensureIsActive();
  markAsUsedImpl();
}

void MetaAlert::markAsUnused()
{
  t_.ensureIsActive();
  markAsUnusedImpl();
}

void MetaAlert::updateSeverityDelta(double delta)
{
  t_.ensureIsActive();
  updateSeverityDeltaImpl(delta);

  assert(ma_.get()!=NULL);
  ma_->updateSeverityDelta(delta);
}

void MetaAlert::updateCertanityDelta(double delta)
{
  t_.ensureIsActive();
  updateCertanityDeltaImpl(delta);

  assert(ma_.get()!=NULL);
  ma_->updateCertanityDelta(delta);
}

void MetaAlert::addChild(Persistency::MetaAlertPtrNN child)
{
  t_.ensureIsActive();
  addChildImpl(child);
}

void MetaAlert::associateWithAlert(Persistency::AlertPtrNN alert)
{
  t_.ensureIsActive();
  associateWithAlertImpl(alert);
}

const Persistency::MetaAlert &MetaAlert::get(void) const
{
  assert(ma_.get()!=NULL);
  return *ma_;
}

} // namespace IO
} // namespace Persistency
