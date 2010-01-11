/*
 * MetaAlert.hpp
 *
 */
#include "Persistency/IO/Postgres/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

MetaAlert::MetaAlert(Persistency::MetaAlertPtrNN  ma,
                     const Transaction           &t,
                     DBHandlerPtrNN               dbHandler):
  IO::MetaAlert(ma, t),
  dbHandler_(dbHandler)
{
}

void MetaAlert::saveImpl(void)
{
  // TODO
}

void MetaAlert::markAsUsedImpl(void)
{
  // TODO
}

void MetaAlert::markAsUnusedImpl(void)
{
  // TODO
}

void MetaAlert::updateSeverityDeltaImpl(double /*delta*/)
{
  // TODO
}

void MetaAlert::updateCertanityDeltaImpl(double /*delta*/)
{
  // TODO
}

void MetaAlert::addChildImpl(Persistency::MetaAlertPtrNN /*child*/)
{
  // TODO
}

void MetaAlert::associateWithAlertImpl(Persistency::AlertPtrNN /*alert*/)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
