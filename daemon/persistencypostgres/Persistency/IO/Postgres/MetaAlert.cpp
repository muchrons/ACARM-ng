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
    EntrySaver                  es(t, *dbHandler_);
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "
                        <<getID()<<" as triggered, trigger name: " <<name;
    es.markMetaAlertAsTriggered(getID(), name);
  TRYCATCH_END
}

void MetaAlert::markAsUsedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver                  es(t, *dbHandler_);
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "<<getID()<<" as used";
    es.markMetaAlertAsUsed( getID() );
  TRYCATCH_END
}

void MetaAlert::markAsUnusedImpl(Transaction &t)
{
  TRYCATCH_BEGIN
    EntrySaver                    es(t, *dbHandler_);
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"mark Meta Alert with ID: "<<getID()<<" as unused";
    es.removeMetaAlertFromTriggered( getID() );
    es.markMetaAlertAsUnused( getID() );
  TRYCATCH_END
}

void MetaAlert::updateSeverityDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver                    es(t, *dbHandler_);
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"update severity delta for Meta Alert with ID: "<<getID()
                        <<" to: "<<delta;
    es.updateSeverityDelta(getID(), delta);
  TRYCATCH_END
}

void MetaAlert::updateCertaintyDeltaImpl(Transaction &t, double delta)
{
  TRYCATCH_BEGIN
    EntrySaver                    es(t, *dbHandler_);
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"update certainty delta for Meta Alert with ID: "<<getID()
                        <<" to: "<<delta;
    es.updateCertaintyDelta(getID(), delta);
  TRYCATCH_END
}

void MetaAlert::addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID childID = dbHandler_->getIDCache()->get( child );
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"add child with ID: "<<childID<<" to Meta Alert with ID: "<<getID();
    es.saveMetaAlertsTree(getID(), childID);
  TRYCATCH_END
}

void MetaAlert::associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandler_);
    const DataBaseID alertID = dbHandler_->getIDCache()->get( alert );
    // TODO: getID()'s result is used multiple times - make this const variable and reuse.
    LOGMSG_DEBUG_S(log_)<<"associate ALert with ID: "<<alertID<<"with Meta Alert with ID: "<<getID();
    es.saveAlertToMetaAlertMap(alertID, getID() );
  TRYCATCH_END
}

DataBaseID MetaAlert::getID()
{
  return dbHandler_->getIDCache()->get( get() );
}
} // namespace Postgres
} // namespace IO
} // namespace Persistency
