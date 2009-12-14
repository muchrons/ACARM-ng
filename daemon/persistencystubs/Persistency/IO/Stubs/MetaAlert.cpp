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

MetaAlert::MetaAlert(Persistency::MetaAlertPtr  ma,
                     const Transaction         &t,
                     int                        handler):
  IO::MetaAlert(ma, t),
  saveCalls_(0),
  markAsUsedCalls_(0),
  markAsUnusedCalls_(0),
  updateSeverityDeltaCalls_(0),
  updateCertanityDeltaCalls_(0),
  addChildCalls_(0),
  assoicateWithAlertCalls_(0)
{
  assert(handler==42);
  handler=0;
}

void MetaAlert::saveImpl()
{
  ++saveCalls_;
}

void MetaAlert::markAsUsedImpl()
{
  ++markAsUsedCalls_;
}

void MetaAlert::markAsUnusedImpl()
{
  ++markAsUnusedCalls_;
}

void MetaAlert::updateSeverityDeltaImpl(double /*delta*/)
{
  ++updateSeverityDeltaCalls_;
}

void MetaAlert::updateCertanityDeltaImpl(double /*delta*/)
{
  ++updateCertanityDeltaCalls_;
}

void MetaAlert::addChildImpl(Persistency::MetaAlertPtr /*child*/)
{
  ++addChildCalls_;
}

void MetaAlert::associateWithAlertImpl(Persistency::AlertPtr /*alert*/)
{
  ++assoicateWithAlertCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
