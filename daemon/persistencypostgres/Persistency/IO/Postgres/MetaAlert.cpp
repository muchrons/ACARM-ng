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
  // TODO: const's missing
  DataBaseID                    maID = es.saveMetaAlert(ma);
  dbHandler_->getIDCache()->add(get() , maID);
  // TODO tests
}

void MetaAlert::markAsTriggeredImpl(Transaction &t, const std::string &name)
{
  // TODO
  EntrySaver                    es(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsTriggered(malertID, name);
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUsed( malertID );
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUnused( malertID );

}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  // TODO tests
  // TODO: jest - this should be implemented as a sinle sql UPDATE statement.
  // maybe there shoul be only EntrySaver object which updates SeverityDelta
  EntrySaver                    es(t, *dbHandler_);
  EntryReader                   er(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID                    malertID = dbHandler_->getIDCache()->get( get() );
  double                        actualSeverity = er.getSeverityDelta(malertID);
  es.updateSeverityDelta(malertID, actualSeverity + delta);
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  // TODO tests
  // TODO: jest - this should be implemented as a sinle sql UPDATE statement.
  // as above
  EntrySaver                    es(t, *dbHandler_);
  EntryReader                   er(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID                    malertID = dbHandler_->getIDCache()->get( get() );
  double                        actualCertainty = er.getCertaintyDelta(malertID);
  es.updateCertaintyDelta(malertID, actualCertainty + delta);

}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  // TODO tests
  EntrySaver es(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID nodeID = dbHandler_->getIDCache()->get( get() );
  // TODO: const's missing
  DataBaseID childID = dbHandler_->getIDCache()->get( child );
  es.saveMetaAlertsTree(nodeID, childID);
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  // TODO: tests
  EntrySaver es(t, *dbHandler_);
  // TODO: const's missing
  DataBaseID maID = dbHandler_->getIDCache()->get( get() );
  // TODO: const's missing
  DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
  es.saveAlertToMetaAlertMap(alertID, maID);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
