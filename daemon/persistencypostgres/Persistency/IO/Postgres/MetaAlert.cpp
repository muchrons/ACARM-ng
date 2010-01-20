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
                     Transaction                 &t,
                     DBHandlerPtrNN               dbHandler):
  IO::MetaAlert(ma, t),
  dbHandler_(dbHandler)
{
}

void MetaAlert::saveImpl(Transaction &)
{
  // TODO
}

void MetaAlert::markAsTriggeredImpl(Transaction &, const std::string &/*name*/)
{
  // TODO
}

void MetaAlert::markAsUsedImpl(Transaction &)
{
  // TODO
}

void MetaAlert::markAsUnusedImpl(Transaction &)
{
  // TODO
}

void MetaAlert::updateSeverityDeltaImpl(Transaction &, double /*delta*/)
{
  // TODO
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &, double /*delta*/)
{
  // TODO
}

void MetaAlert::addChildImpl(Transaction &, Persistency::MetaAlertPtrNN /*child*/)
{
  // TODO
}

void MetaAlert::associateWithAlertImpl(Transaction &, Persistency::AlertPtrNN /*alert*/)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
