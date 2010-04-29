/*
 * MetaAlert.cpp
 *
 */
#include "Persistency/IO/Postgres/MetaAlert.hpp"
using namespace Persistency::IO::Postgres::detail;

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

void MetaAlert::saveImpl(Transaction &t)
{
  EntrySaver                    es(t, *dbHandler_);
  const Persistency::MetaAlert &ma = *get();
  const DataBaseID                    maID = es.saveMetaAlert(ma);
  dbHandler_->getIDCache()->add(get() , maID);
}

void MetaAlert::markAsTriggeredImpl(Transaction &t, const std::string &name)
{
  EntrySaver                  es(t, *dbHandler_);
  es.markMetaAlertAsTriggered(getID(), name);
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  EntrySaver                  es(t, *dbHandler_);
  es.markMetaAlertAsUsed( getID() );
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  EntrySaver                    es(t, *dbHandler_);
  es.markMetaAlertAsUnused( getID() );

}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  EntrySaver                    es(t, *dbHandler_);
  es.updateSeverityDelta(getID(), delta);
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  EntrySaver                    es(t, *dbHandler_);
  es.updateCertaintyDelta(getID(), delta);

}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  EntrySaver       es(t, *dbHandler_);
  const DataBaseID childID = dbHandler_->getIDCache()->get( child );
  es.saveMetaAlertsTree(getID(), childID);
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  EntrySaver       es(t, *dbHandler_);
  const DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
  es.saveAlertToMetaAlertMap(alertID, getID() );
}

DataBaseID MetaAlert::getID()
{
  return dbHandler_->getIDCache()->get( get() );
}
} // namespace Postgres
} // namespace IO
} // namespace Persistency
