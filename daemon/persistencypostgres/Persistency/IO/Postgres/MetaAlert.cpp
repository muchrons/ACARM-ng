/*
 * MetaAlert.hpp
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
  DataBaseID                    maID = es.saveMetaAlert(ma);
  dbHandler_->getIDCache()->add(get() , maID);
  // TODO tests
}

void MetaAlert::markAsTriggeredImpl(Transaction &, const std::string &/*name*/)
{
  // TODO
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUsed( malertID );
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUnused( malertID );

}

void MetaAlert::updateSeverityDeltaImpl(Transaction &, double /*delta*/)
{
  // TODO
  // add to the severity in data base
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &, double /*delta*/)
{
  // TODO
  // add to the certainty in data base
}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  // TODO tests
  EntrySaver es(t, *dbHandler_);
  DataBaseID nodeID = dbHandler_->getIDCache()->get( get() );
  DataBaseID childID = dbHandler_->getIDCache()->get( child );
  es.saveMetaAlertsTree(nodeID, childID);
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  // TODO: tests
  EntrySaver es(t, *dbHandler_);
  DataBaseID maID = dbHandler_->getIDCache()->get( get() );
  DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
  es.saveAlertToMetaAlertMap(alertID, maID);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
