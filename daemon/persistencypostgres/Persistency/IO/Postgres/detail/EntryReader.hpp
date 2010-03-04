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
  /** \brief create reader
   *  \param t   transaction to be used for reading.
   *  \param dbh data base handler object.
   */
  EntryReader(Transaction &t, DBHandler &dbh);
  //TODO: work in progress
  Persistency::AlertPtrNN readAlert(DataBaseID alertID);

  Alert::SourceAnalyzers getAnalyzers(DataBaseID alertID);

private:
  AnalyzerPtrNN getAnalyzer(DataBaseID anlzID);
  Alert::ReportedHosts getReporteHosts(DataBaseID alertID, std::string hostType);
  HostPtr getHost(DataBaseID hostID, DataBaseID refID);

  Persistency::Host::ReportedServices  getReportedServices(DataBaseID hostID);
  Persistency::Host::ReportedProcesses getReportedProcesses(DataBaseID hostID);
  ReferenceURLPtr getReferenceURL(DataBaseID refID);

  Persistency::ProcessPtr getProcess(DataBaseID procID, DataBaseID refID);
  Persistency::ServicePtr getService(DataBaseID servID, DataBaseID refID);

  Alert::ReportedHosts getSourceHosts(DataBaseID alertID);
  Alert::ReportedHosts getTargetHosts(DataBaseID alertID);

  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntryReader

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency


#endif
