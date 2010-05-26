/*
 * MetaAlert.cpp
 *
 */
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

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
  log_("persistency.io.postgres.metaalert"),
  dbHandler_(dbHandler)
{
}

void MetaAlert::saveImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver                    es(t, *dbHandler_);
    const Persistency::MetaAlert &ma = *get();
    const DataBaseID              maID = es.saveMetaAlert(ma);
    dbHandler_->getIDCache()->add(get() , maID);
  TRYCATCH_END
}

void MetaAlert::markAsTriggeredImpl(Transaction &t, const std::string &name)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "
                        <<malertID<<" as triggered, trigger name: " <<name;
    es.markMetaAlertAsTriggered(malertID, name);
  TRYCATCH_END
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "<<malertID<<" as used";
    es.markMetaAlertAsUsed( malertID );
  TRYCATCH_END
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "<<malertID<<" as unused";
    es.markMetaAlertAsUnused( malertID );
  TRYCATCH_END
}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID malertID = getID();
    // TODO: comment is not precise - update here is "by" not "to"
    LOGMSG_DEBUG_S(log_)<<"update severity delta for Meta Alert with ID: "<<malertID
                        <<" to: "<<delta;
    es.updateSeverityDelta(malertID, delta);
  TRYCATCH_END
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID malertID = getID();
    // TODO: comment is not precise - update here is "by" not "to"
    LOGMSG_DEBUG_S(log_)<<"update certainty delta for Meta Alert with ID: "<<malertID
                        <<" to: "<<delta;
    es.updateCertaintyDelta(malertID, delta);
  TRYCATCH_END
}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID childID = dbHandler_->getIDCache()->get( child );
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"add child with ID: "<<childID<<" to Meta Alert with ID: "<<malertID;
    es.saveMetaAlertsTree(malertID, childID);
  TRYCATCH_END
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"associate ALert with ID: "<<alertID<<"with Meta Alert with ID: "<<malertID;
    es.saveAlertToMetaAlertMap(alertID, malertID );
  TRYCATCH_END
}

DataBaseID MetaAlert::getID()
{
  return dbHandler_->getIDCache()->get( get() );
}
} // namespace Postgres
} // namespace IO
} // namespace Persistency
