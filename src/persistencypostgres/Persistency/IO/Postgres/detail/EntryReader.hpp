/*
 * EntryReader.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYREADER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYREADER_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/Process.hpp"
#include "Persistency/Service.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/DataBaseID.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"
#include "Persistency/IO/Postgres/ExceptionNoEntries.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

/** \brief helper classs to read particular elements from data base
 */
class EntryReader: private boost::noncopyable
{
public:
  /** \brief create reader
   *  \param t   transaction to be used for reading.
   *  \param dbh data base handle object.
   */
  EntryReader(Transaction &t, DBHandle &dbh);

  /** \brief read Alert data from data base
   *  \param alertID ID of Alert in data base
   *  \return return alert.
   */
  Persistency::AlertPtrNN readAlert(DataBaseID alertID);

  /** \brief read Meta Alert data from data base
   *  \param malertID ID of Meta Alert in data base
   *  \return return meta alert.
   */
  Persistency::MetaAlertPtrNN readMetaAlert(DataBaseID malertID);

  /** \brief read Analyzers assiociated with Alert from data base.
   *  \param alertID ID of Alert in data base.
   *  \return return source analyzers.
   */
  Persistency::Alert::Analyzers getAnalyzers(DataBaseID alertID);

  /** \brief read value of Meta Alert severity delta
   *  \param malertID ID of Meta Alert
   *  \return return severity delta.
   */
  double getSeverityDelta(DataBaseID malertID);

  /** \brief read value of Meta Alert certainty delta
   *  \param malertID ID of Meta Alert
   *  \return return certainty delta.
   */
  double getCertaintyDelta(DataBaseID malertID);

  /** \brief read data associated with leafs in meta alerts tree
   *  \param malertID ID of Meta Alert associated with Alert
   *  \return return leaf.
   */
  Persistency::AlertPtrNN getLeaf(DataBaseID malertID);

  /** \brief read Analyzers' data from data base
   *  \param  anlzID ID of Analyzer
   *  \return analyzers' data
   */
  AnalyzerPtrNN getAnalyzer(DataBaseID anlzID);

  /** \brief  read meta alerts' children ID's from data base
   *  \param  malertID ID of Meta Alert
   *  \return vector which contains IDs of Meta Alert children
   */
  std::vector<DataBaseID> readMetaAlertChildren(DataBaseID malertID);

  /** \brief read IDs of Meta Alerts in use
   *  \return vector which contains IDs of Meta alerts in use
   */
  std::vector<DataBaseID> readIDsMalertsInUse();

  /** \brief read IDs of Meta Alerts from time interval
   *  \param  from begin of time interval
   *  \param  to end of time interval
   *  \return vector which contains IDs of Meta alerts from time interval
   */
  std::vector<DataBaseID> readIDsMalertsBetween(const Timestamp &from, const Timestamp &to);

  /** \brief read IDs of root nodes
  *  \return vector of IDs
  */
  std::vector<DataBaseID> readRoots();

  /** \brief  read meta alerts' children ID's from data base for all meta alerts
   *  \return map of IDs
   */
  std::map<DataBaseID, std::vector<DataBaseID> > readAllMetaAlertsChildren();
  /** \brief read IDs of root nodes
   *  \param  from begin of time interval
   *  \param  to end of time interval
   *  \return vector of IDs
   */
  std::vector<DataBaseID> readRoots(const Timestamp &from, const Timestamp &to);

  /** \brief get ID of Alert asssiciated with Meata Alert
   *  \param malertID ID of Meta Alert
   *  \return return alert ID.
   */
  DataBaseID getAlertIDAssociatedWithMetaAlert(DataBaseID malertID);

  /** \brief reads system-ID of metal alert of given database-ID.
   *  \param malertID database-ID of meta-alert to read system-ID for.
   *  \return system-ID of a given meta-alert.
   */
  Persistency::MetaAlert::ID getSystemIDOfMetaAlert(DataBaseID malertID);

  /** \brief get configuration paramter.
   *  \param owner owner of the entry.
   *  \param key   key to read value from.
   *  \return value corresponding to given entry or NULL.
   */
  DynamicConfig::ValueNULL readConfigParameter(const DynamicConfig::Owner &owner,
                                               const DynamicConfig::Key   &key);

  /** \brief get configuration paramter.
   *  \param owner owner of the entry.
   *  \param key   key to read value from.
   *  \return value corresponding to given entry.
   */
  DynamicConfig::Value readConstConfigParameter(const DynamicConfig::Owner &owner,
                                                const DynamicConfig::Key   &key);

  /** \brief iterates over given owner's configuration paramters.
   *  \param owner owner of the entry.
   *  \param cb    callback to be called for each iteration.
   */
  void iterateConfigParameters(const DynamicConfig::Owner &owner, DynamicConfig::IterationCallback &cb);

private:
  DynamicConfig::ValueNULL readConfigParameterCommon(const char                 *table,
                                                     const DynamicConfig::Owner &owner,
                                                     const DynamicConfig::Key   &key);

  Persistency::Alert::Hosts getReporteHosts(DataBaseID alertID, std::string hostType);

  Persistency::Host::Services  getServices(DataBaseID hostID);
  Persistency::Host::Processes getProcesses(DataBaseID hostID);
  ReferenceURLPtr getReferenceURL(const DataBaseID *refID);

  Persistency::Alert::Hosts getSourceHosts(DataBaseID alertID);
  Persistency::Alert::Hosts getTargetHosts(DataBaseID alertID);
  template<typename T>
  void addIfNew(T e, DataBaseID id);
  std::vector<DataBaseID> getRoots(const pqxx::result &r);

  Logger::Node log_;
  DBHandle    &dbh_;
  Transaction &t_;
}; // class EntryReader

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency


#endif
