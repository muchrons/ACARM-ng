/*
 * MetaAlert.cpp
 *
 */
#include "Persistency/IO/Stubs/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

MetaAlert::MetaAlert(Persistency::MetaAlertPtr  ma,
                     const Transaction         &t):
  IO::MetaAlert(ma, t)
{
}

void MetaAlert::saveImpl()
{
}

void MetaAlert::markAsUsedImpl()
{
}

void MetaAlert::markAsUnusedImpl()
{
}

void MetaAlert::updateSeverityDeltaImpl(double /*delta*/)
{
}

void MetaAlert::updateCertanityDeltaImpl(double /*delta*/)
{
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
