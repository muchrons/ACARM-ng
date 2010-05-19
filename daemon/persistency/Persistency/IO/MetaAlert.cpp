/*
 * MetaAlert.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
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
  saveImpl(t_);
}

void MetaAlert::markAsTriggered(const std::string &name)
{
  t_.ensureIsActive();
  markAsTriggeredImpl(t_, name);
}

void MetaAlert::markAsUsed()
{
  t_.ensureIsActive();
  markAsUsedImpl(t_);
}

void MetaAlert::markAsUnused()
{
  t_.ensureIsActive();
  markAsUnusedImpl(t_);
}

void MetaAlert::updateSeverityDelta(double delta)
{
  t_.ensureIsActive();
  updateSeverityDeltaImpl(t_, delta);

  assert(ma_.get()!=NULL);
  ma_->updateSeverityDelta(delta);
}

void MetaAlert::updateCertaintyDelta(double delta)
{
  t_.ensureIsActive();
  updateCertaintyDeltaImpl(t_, delta);

  assert(ma_.get()!=NULL);
  ma_->updateCertaintyDelta(delta);
}

void MetaAlert::addChild(Persistency::MetaAlertPtrNN child)
{
  t_.ensureIsActive();
  addChildImpl(t_, child);
}

void MetaAlert::associateWithAlert(Persistency::AlertPtrNN alert)
{
  t_.ensureIsActive();
  associateWithAlertImpl(t_, alert);
}

Persistency::MetaAlertPtrNN MetaAlert::get(void)
{
  assert(ma_.get()!=NULL);
  return ma_;
}

} // namespace IO
} // namespace Persistency
