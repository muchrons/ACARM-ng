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
                     DBHandlePtrNN                dbHandle):
  IO::MetaAlert(ma, t),
  log_("persistency.io.postgres.metaalert"),
  dbHandle_(dbHandle)
{
}

void MetaAlert::saveImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver                    es(t, *dbHandle_);
    const Persistency::MetaAlert &ma = *get();
    const DataBaseID              maID = es.saveMetaAlert(ma);
    dbHandle_->getIDCache()->add(get() , maID);
    es.saveRootID(maID);
  TRYCATCH_END
}

void MetaAlert::markAsTriggeredImpl(Transaction &t, const std::string &name)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with db-ID: "
                        <<malertID<<" as triggered, trigger name: " <<name;
    es.markMetaAlertAsTriggered(malertID, name);
  TRYCATCH_END
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with db-ID: "<<malertID<<" as used";
    es.markMetaAlertAsUsed( malertID );
  TRYCATCH_END
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with db-ID: "<<malertID<<" as unused";
    es.markMetaAlertAsUnused( malertID );
  TRYCATCH_END
}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"update severity delta for Meta Alert with db-ID: "<<malertID
                        <<" by: "<<delta;
    es.updateSeverityDelta(malertID, delta);
  TRYCATCH_END
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"update certainty delta for Meta Alert with db-ID: "<<malertID
                        <<" by: "<<delta;
    es.updateCertaintyDelta(malertID, delta);
  TRYCATCH_END
}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID childID = dbHandle_->getIDCache()->get( child );
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"add child with db-ID: "<<childID<<" to Meta Alert with db-ID: "<<malertID;
    es.saveMetaAlertsTree(malertID, childID);
    es.deleteRootID(childID);
  TRYCATCH_END
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    const DataBaseID alertID = dbHandle_->getIDCache()->get( alert );
    const DataBaseID malertID = getID();
    LOGMSG_DEBUG_S(log_)<<"associate ALert with db-ID: "<<alertID<<" with Meta Alert with db-ID: "<<malertID;
    es.saveAlertToMetaAlertMap(alertID, malertID );
  TRYCATCH_END
}

DataBaseID MetaAlert::getID()
{
  return dbHandle_->getIDCache()->get( get() );
}
} // namespace Postgres
} // namespace IO
} // namespace Persistency
