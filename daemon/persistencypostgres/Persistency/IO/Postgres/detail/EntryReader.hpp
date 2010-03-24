/*
 * EntryReader.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYREADER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYREADER_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Persistency/Process.hpp"
#include "Persistency/Service.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Postgres/DataBaseID.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IPTypes.hpp"

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

  typedef std::map<DataBaseID, Persistency::AlertPtrNN> leafsMap;
  typedef std::multimap<DataBaseID, std::pair<DataBaseID, Persistency::GraphNodePtrNN> > nodesMultimap;
  /** \brief create reader
   *  \param t   transaction to be used for reading.
   *  \param dbh data base handler object.
   */
  EntryReader(Transaction &t, DBHandler &dbh);

  /** \brief read Alert data from data base
   *  \param alertID ID of Alert in data base
   */
  Persistency::AlertPtrNN readAlert(DataBaseID alertID);

  /** \brief read Meta Alert data from data base
   *  \param maletrID ID of Meta Alert in data base
   */
  Persistency::MetaAlertPtrNN readMetaAlert(DataBaseID malertID);

  /** \brief read Analyzers assiociated with Alert from data base
   *  \param alertID ID of Alert in data base
   */
  Persistency::Alert::SourceAnalyzers getAnalyzers(DataBaseID alertID);

  /** \brief read value of Meta Alert severity delta
   *  \param malertID ID of Meta Alert
   */
  double getSeverityDelta(DataBaseID malertID);

  /** \brief read value of Meta Alert certainty delta
   *  \param malertID ID of Meta Alert
   */
  double getCertaintyDelta(DataBaseID malertID);

  /** \brief get number of Meta Alert children in meta alerts tree
   *  \param  malertID ID of Mata Alert
   *  \return number of children of Meta Alert
   */
  size_t getChildrenIDs(DataBaseID malertID);

  /** \brief read data associated with leafs in meta alerts tree
   *  \param malertID ID of Meta Alert associated with Alert
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
private:
  Persistency::Alert::ReportedHosts getReporteHosts(DataBaseID alertID, std::string hostType);
  HostPtr getHost(DataBaseID hostID, DataBaseID *refID);

  Persistency::Host::ReportedServices  getReportedServices(DataBaseID hostID);
  Persistency::Host::ReportedProcesses getReportedProcesses(DataBaseID hostID);
  ReferenceURLPtr getReferenceURL(DataBaseID *refID);

  Persistency::ProcessPtr getProcess(DataBaseID procID, DataBaseID *refID);
  Persistency::ServicePtr getService(DataBaseID servID, DataBaseID *refID);

  Persistency::Alert::ReportedHosts getSourceHosts(DataBaseID alertID);
  Persistency::Alert::ReportedHosts getTargetHosts(DataBaseID alertID);

  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntryReader

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency


#endif
