/*
 * MetaAlert.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Stubs/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

MetaAlert::MetaAlert(Persistency::MetaAlertPtrNN  ma,
                     Transaction                 &t,
                     int                          handler):
  IO::MetaAlert(ma, t),
  saveCalls_(0),
  markAsTriggeredCalls_(0),
  markAsUsedCalls_(0),
  markAsUnusedCalls_(0),
  updateSeverityDeltaCalls_(0),
  updateCertaintyDeltaCalls_(0),
  addChildCalls_(0),
  assoicateWithAlertCalls_(0)
{
  assert(handler==42);
  handler=0;
}

void MetaAlert::saveImpl(Transaction &)
{
  ++saveCalls_;
}

void MetaAlert::markAsTriggeredImpl(Transaction &, const std::string &)
{
  ++markAsTriggeredCalls_;
}

void MetaAlert::markAsUsedImpl(Transaction &)
{
  ++markAsUsedCalls_;
}

void MetaAlert::markAsUnusedImpl(Transaction &)
{
  ++markAsUnusedCalls_;
}

void MetaAlert::updateSeverityDeltaImpl(Transaction &, double /*delta*/)
{
  ++updateSeverityDeltaCalls_;
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &, double /*delta*/)
{
  ++updateCertaintyDeltaCalls_;
}

void MetaAlert::addChildImpl(Transaction &, Persistency::MetaAlertPtrNN /*child*/)
{
  ++addChildCalls_;
}

void MetaAlert::associateWithAlertImpl(Transaction &, Persistency::AlertPtrNN /*alert*/)
{
  ++assoicateWithAlertCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
