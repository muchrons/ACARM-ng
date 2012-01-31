/*
 * EntryReader.cpp
 *
 */
#include <sstream>
#include <ctime>
#include <cassert>
#include <boost/algorithm/string.hpp>

#include "Base/NullValue.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/SQLHelper.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using namespace std;
using namespace pqxx;

using boost::algorithm::trim;
using namespace Persistency;
using namespace Base;
using Persistency::IO::Transaction;

// this is helper macro for calling f-cjtion that saves line number and calls given sql statement (with log)
#define SQL(sql, log) SQLHelper(__FILE__, __LINE__, (sql), (log))

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

namespace
{

SeverityLevel severityFromInt(const DataBaseID id)
{
  // translate to enum
  switch(id)
  {
    case 0: return SeverityLevel::DEBUG;
    case 1: return SeverityLevel::INFO;
    case 2: return SeverityLevel::LOW;
    case 3: return SeverityLevel::MEDIUM;
    case 4: return SeverityLevel::HIGH;
  }
  // when we reach here, there is wrong severity level in data base
  assert(!"invalid severity level");
  return SeverityLevel::HIGH;
}

} // unnamed namespace


EntryReader::EntryReader(Transaction &t, DBHandle &dbh):
  log_("persistency.io.postgres.detail.entryreader"),
  dbh_(dbh),
  t_(t)
{
}

Persistency::AlertPtrNN EntryReader::readAlert(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alerts WHERE id = " << alertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  AlertPtrNN alert( new Alert(ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                              getAnalyzers( alertID ),
                              ReaderHelper< Base::NullValue<Timestamp> >::readAs(r[0]["detect_time"]).get(),
                              timestampFromString( ReaderHelper<string>::readAsNotNull(r[0]["create_time"]) ),
                              Severity( severityFromInt(ReaderHelper<DataBaseID>::readAsNotNull(r[0]["severity"]) ) ),
                              Certainty( ReaderHelper<double>::readAsNotNull(r[0]["certainty"]) ),
                              ReaderHelper<string>::readAsNotNull(r[0]["description"]),
                              getSourceHosts(alertID),
                              getTargetHosts(alertID) ) );
  return alert;
}

Persistency::MetaAlertPtrNN EntryReader::readMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  const string CreateTime( ReaderHelper<string>::readAsNotNull(r[0]["create_time"]) );
  MetaAlertPtrNN malert( new Persistency::MetaAlert( ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                                     ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]),
                                                     ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]),
                                                     getReferenceURL( ReaderHelper< NullValue<DataBaseID> >::readAs(r[0]["id_ref"]).get() ),
                                                     timestampFromString( CreateTime ),
                                                     ReaderHelper<Persistency::MetaAlert::ID::Numeric>::readAsNotNull(r[0]["sys_id"]) ) );
  return malert;
}

AnalyzerPtrNN EntryReader::getAnalyzer(DataBaseID anlzID)
{
  stringstream sa;
  sa << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  const result ra = SQL( sa.str(), log_ ).exec(t_);
  if(ra.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, sa.str());
  AnalyzerPtrNN anlz(new Analyzer( ReaderHelper<Analyzer::ID::Numeric>::readAsNotNull(ra[0]["sys_id"]),
                                   Analyzer::Name( ReaderHelper<string>::readAsNotNull(ra[0]["name"]) ),
                                   ReaderHelper<Analyzer::Version>::readAs(ra[0]["version"]),
                                   ReaderHelper<Analyzer::OperatingSystem>::readAs(ra[0]["os"]),
                                   ReaderHelper< Base::NullValue<Analyzer::IP> >::readAs(ra[0]["ip"]).get() ));
  return anlz;
}

Alert::Analyzers EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  // add first element to the Analyzers
  Alert::Analyzers analyzers( getAnalyzer( ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_analyzer"]) ) );
  // enumerating starts from '1' because first element was added to the Analyzers above
  for(size_t i=1; i<r.size(); ++i)
    analyzers.push_back( getAnalyzer( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_analyzer"]) ) );
  return analyzers;
}

Alert::Hosts EntryReader::getReporteHosts(DataBaseID alertID, std::string hostType)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "SELECT * FROM hosts WHERE id_alert = "<< alertID <<" AND role = ";
  ap.append(ss, hostType);
  const result r = SQL( ss.str(), log_ ).exec(t_);
  Alert::Hosts hosts;
  for(size_t i=0; i<r.size(); ++i)
  {
    const DataBaseID hostID=ReaderHelper<DataBaseID>::readAsNotNull( r[i]["id"] );
    HostPtrNN host(new Persistency::Host( ReaderHelper<Persistency::Host::IP>::readAsNotNull(r[i]["ip"]),
                                          ReaderHelper< Base::NullValue<Persistency::Host::Netmask> >::readAs(r[i]["mask"]).get(),
                                          ReaderHelper<Persistency::Host::OperatingSystem>::readAs(r[i]["os"]),
                                          getReferenceURL( ReaderHelper<NullValue<DataBaseID> >::readAs( r[i]["id_ref"] ).get() ),
                                          getServices(hostID),
                                          getProcesses(hostID),
                                          ReaderHelper<Persistency::Host::Name>::readAs(r[i]["name"]) ) );
    // add host to cache
    addIfNew(host, hostID);
    hosts.push_back(host);
  } // for(hosts)
  return hosts;
}

Alert::Hosts EntryReader::getSourceHosts(DataBaseID alertID)
{
  return getReporteHosts(alertID, "src");
}

Alert::Hosts EntryReader::getTargetHosts(DataBaseID alertID)
{
  return getReporteHosts(alertID, "dst");
}

Persistency::Host::Services EntryReader::getServices(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM services WHERE id_host = " << hostID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  Persistency::Host::Services services;
  for(size_t i = 0;i < r.size(); ++i)
  {
    ServicePtrNN service( new Service( ReaderHelper<string>::readAsNotNull(r[i]["name"]),
                                       ReaderHelper<DataBaseID>::readAsNotNull(r[i]["port"]),
                                       ReaderHelper<Service::Protocol>::readAs(r[i]["protocol"]),
                                       getReferenceURL( ReaderHelper<NullValue<DataBaseID> >::readAs( r[i]["id_ref"] ).get() ) ) );
    services.push_back(service);
  } // for(services)
  return services;
}

Persistency::Host::Processes EntryReader::getProcesses(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM procs WHERE id_host = " << hostID;
  const result r = SQL( ss.str(), log_ ).exec(t_);

  Persistency::Host::Processes processes;
  for(size_t i = 0; i<r.size(); ++i)
  {
    const NullValue<string> params  =ReaderHelper< NullValue<string> >::readAs(r[i]["arguments"]);
    const NullValue<string> username=ReaderHelper< NullValue<string> >::readAs(r[i]["username"]);
    Persistency::ProcessPtr process( new Process( ReaderHelper<Process::Path>::readAs(r[i]["path"]),
                                                  ReaderHelper<string>::readAsNotNull(r[i]["name"]),
                                                  ReaderHelper< std::auto_ptr<MD5Sum> >::readAs(r[i]["md5"]).get(),
                                                  ReaderHelper< NullValue<pid_t> >::readAs(r[i]["pid"]).get(),
                                                  ReaderHelper< NullValue<int> >::readAs(r[i]["uid"]).get(),
                                                  (username.get()==NULL)?NULL:username.get()->c_str(),
                                                  (params.get()==NULL)?NULL:params.get()->c_str(),
                                                  getReferenceURL( ReaderHelper<NullValue<DataBaseID> >::readAs( r[i]["id_ref"] ).get() ) ) );
    processes.push_back(process);
  }
  return processes;
}

ReferenceURLPtr EntryReader::getReferenceURL(const DataBaseID *refID)
{
  ReferenceURLPtr refURLPtr;
  if(refID == NULL)
    return refURLPtr;
  stringstream ss;
  ss << "SELECT * FROM reference_urls WHERE id = " << *refID;
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());

  refURLPtr = ReferenceURLPtr( new ReferenceURL(ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                                ReaderHelper<string>::readAsNotNull(r[0]["url"])) );
  return refURLPtr;
}

double EntryReader::getSeverityDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT severity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]);
}
double EntryReader::getCertaintyDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT certainty_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]);
}

Persistency::AlertPtrNN EntryReader::getLeaf(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return Persistency::AlertPtrNN( readAlert( ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_alert"]) ) );
}

vector<DataBaseID> EntryReader::readMetaAlertChildren(DataBaseID malertID)
{
  vector<DataBaseID> childrenIDs;
  stringstream ss;
  ss << "SELECT id_child FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  childrenIDs.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
    childrenIDs.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_child"]) );
  return childrenIDs;
}

vector<DataBaseID> EntryReader::readIDsMalertsInUse()
{
  vector<DataBaseID> malertsInUse;
  stringstream ss;
  ss << "SELECT id_meta_alert FROM meta_alerts_in_use;";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  malertsInUse.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
  {
    //assert that id_meta_alert is not NULL
    assert(!r[i]["id_meta_alert"].is_null());
    malertsInUse.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_meta_alert"]) );
  }
  return malertsInUse;
}

vector<DataBaseID> EntryReader::readIDsMalertsBetween(const Timestamp &from, const Timestamp &to)
{
  vector<DataBaseID> malertsBetween;
  Appender           ap(t_);
  stringstream       ss;
  ss << "SELECT id FROM meta_alerts WHERE ";
  ap.append(ss, from);
  ss << " <= create_time AND create_time <=";
  ap.append(ss, to);
  ss << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  malertsBetween.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
    malertsBetween.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id"]) );
  return malertsBetween;
}

std::vector<DataBaseID> EntryReader::readRoots()
{
  SQL("CREATE TEMP TABLE tmp ON COMMIT DROP AS SELECT id_node, id_child FROM meta_alerts_tree"
      " INNER JOIN meta_alerts_in_use ON(meta_alerts_tree.id_node=meta_alerts_in_use.id_meta_alert);",
      log_).exec(t_);

  const result r = SQL("SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( "
                       "SELECT id_node FROM tmp S WHERE T.id_node=S.id_child );", log_).exec(t_);

  return getRoots(r);
}

std::map<DataBaseID, std::vector<DataBaseID> > EntryReader::readAllMetaAlertsChildren()
{
  typedef std::vector<DataBaseID>  vectorIDs;
  std::map<DataBaseID,  vectorIDs> allChildrenIDs;
  stringstream ss;
  ss << "SELECT * FROM tmp;";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  for(size_t i=0; i<r.size(); ++i)
  {
    const DataBaseID idNode = ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_node"]);
    const DataBaseID idChild = ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_child"]);
    allChildrenIDs[idNode].push_back(idChild);
  }
  return allChildrenIDs;
}

std::vector<DataBaseID> EntryReader::readRoots(const Timestamp &from, const Timestamp &to)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "CREATE TEMP TABLE tmp ON COMMIT DROP AS SELECT id_node, id_child FROM meta_alerts_tree"
        " INNER JOIN meta_alerts ON(meta_alerts_tree.id_node=meta_alerts.id) WHERE ";
  ap.append(ss, from);
  ss << " <= create_time AND create_time <=";
  ap.append(ss, to);
  ss << ";";
  SQL( ss.str(), log_ ).exec(t_);

  const result r = SQL("SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( "
                       "SELECT id_node FROM tmp S WHERE T.id_node=S.id_child );", log_).exec(t_);

  return getRoots(r);
}

DataBaseID EntryReader::getAlertIDAssociatedWithMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_alert"]);
}

Persistency::MetaAlert::ID EntryReader::getSystemIDOfMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT sys_id FROM meta_alerts WHERE id = " << malertID;
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str() );
  return ReaderHelper<Persistency::MetaAlert::ID::Numeric>::readAsNotNull(r[0]["sys_id"]);
}

DynamicConfig::ValueNULL EntryReader::readConfigParameter(const DynamicConfig::Owner &owner,
                                                          const DynamicConfig::Key   &key)
{
  return readConfigParameterCommon("config", owner, key);
}

DynamicConfig::Value EntryReader::readConstConfigParameter(const DynamicConfig::Owner &owner,
                                                           const DynamicConfig::Key   &key)
{
  const DynamicConfig::ValueNULL tmp=readConfigParameterCommon("config_rdonly", owner, key);
  // if no value - rise an error
  if( tmp.get()==NULL )
    throw DynamicConfig::ExceptionNoSuchParameter(SYSTEM_SAVE_LOCATION, owner, key);
  // if there is any entry, return its value.
  return *tmp.get();
}

DynamicConfig::ValueNULL EntryReader::readConfigParameterCommon(const char                 *table,
                                                                const DynamicConfig::Owner &owner,
                                                                const DynamicConfig::Key   &key)
{
  assert(table!=NULL);
  stringstream ss;
  Appender     ap(t_);
  ss << "SELECT value FROM "<<table<<" WHERE owner=";
  ap.append(ss, owner.get() );
  ss << " AND key=";
  ap.append(ss, key.get() );
  const result r = SQL( ss.str(), log_ ).exec(t_);
  // no entry? return NULL.
  if( r.size()!=1 )
  {
    if( r.size()>1 )
      LOGMSG_ERROR_S(log_)<<"in table '"<<table<<"' key '"<<key.get()<<"' has "<<r.size()
                          <<" entries - returning NULL as there were none...";
    return DynamicConfig::ValueNULL();
  }
  // if there is any entry, return its value.
  assert( r.size()==1 );
  const string &value=ReaderHelper<string>::readAsNotNull(r[0]["value"]);
  return DynamicConfig::ValueNULL(value);
}

void EntryReader::iterateConfigParameters(const DynamicConfig::Owner &owner, DynamicConfig::IterationCallback &cb)
{
  // perform query
  stringstream ss;
  Appender     ap(t_);
  ss << "SELECT key, value FROM config WHERE owner = ";
  ap.append(ss, owner.get());
  const result r = SQL( ss.str(), log_ ).exec(t_);
  // iterate through results
  for(result::const_iterator it=r.begin(); it!=r.end(); ++it)
  {
    const string &key  =ReaderHelper<string>::readAsNotNull( (*it)["key"] );
    const string &value=ReaderHelper<string>::readAsNotNull( (*it)["value"] );
    if( cb.process(key, value)==false )
    {
      LOGMSG_DEBUG(log_, "terminating iteration over elements on callback's request");
      return;
    }
  }
}

template<typename T>
void EntryReader::addIfNew(T e, DataBaseID id)
{
  if(!dbh_.getIDCache()->has(e))
    dbh_.getIDCache()->add(e, id);
  assert(id == dbh_.getIDCache()->get(e));
}

std::vector<DataBaseID> EntryReader::getRoots(const pqxx::result &r)
{
  vector<DataBaseID> roots;
  roots.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
  {
    // assert to ensure value is not null
    assert(!r[i]["id_node"].is_null());
    roots.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_node"]) );
  }
  return roots;
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
