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
  const DataBaseID                    maID = es.saveMetaAlert(ma);
  dbHandler_->getIDCache()->add(get() , maID);
}

void MetaAlert::markAsTriggeredImpl(Transaction &t, const std::string &name)
{
  EntrySaver                  es(t, *dbHandler_);
  const DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsTriggered(malertID, name);
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  EntrySaver                  es(t, *dbHandler_);
  const DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUsed( malertID );
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  EntrySaver                    es(t, *dbHandler_);
  const DataBaseID malertID = dbHandler_->getIDCache()->get( get() );
  es.saveMetaAlertAsUnused( malertID );

}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  const DataBaseID              malertID = dbHandler_->getIDCache()->get( get() );
  es.updateSeverityDelta(malertID, delta);
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  // TODO tests
  EntrySaver                    es(t, *dbHandler_);
  const DataBaseID              malertID = dbHandler_->getIDCache()->get( get() );
  es.updateCertaintyDelta(malertID, delta);

}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  // TODO tests
  EntrySaver       es(t, *dbHandler_);
  const DataBaseID nodeID = dbHandler_->getIDCache()->get( get() );
  const DataBaseID childID = dbHandler_->getIDCache()->get( child );
  es.saveMetaAlertsTree(nodeID, childID);
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  // TODO: tests
  EntrySaver       es(t, *dbHandler_);
  const DataBaseID maID = dbHandler_->getIDCache()->get( get() );
  const DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
  es.saveAlertToMetaAlertMap(alertID, maID);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
