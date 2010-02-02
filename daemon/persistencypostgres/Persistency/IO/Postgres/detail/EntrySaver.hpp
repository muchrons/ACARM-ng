/*
 * EntrySaver.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYSAVER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_ENTRYSAVER_HPP_FILE

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

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

/** \brief helper class to save particular elements in data base.
 */
class EntrySaver: private boost::noncopyable
{
public:
  /** \brief create saver.
   *  \param t   transaction to be used for saving.
   *  \param dbh data base handler object.
   */
  EntrySaver(Transaction &t, DBHandler &dbh);

  /** \brief saves process data to data base.
   *  \param reportedHostID ID of reported host, that this Process should be assigned to.
   *  \param p              process to be saved.
   */
  DataBaseID saveProcess(DataBaseID reportedHostID, const Process &p);
  DataBaseID saveAlert(DataBaseID AnalyzerID, const Alert &a);
  DataBaseID saveAnalyzer(DataBaseID HostID, const AnalyzerPtr a);
  DataBaseID saveDestinationHost(DataBaseID alertID, const HostPtr h);
  DataBaseID saveTargetHost(DataBaseID alertID, const HostPtr h);
  void saveService(DataBaseID reportedHostID, const Service &s);
  DataBaseID saveHostData(const HostPtr h);
  // TODO


private:
  DataBaseID getID(const std::string &seqName);
  DataBaseID getSeverityID(const Alert &a);

  DataBaseID saveProcessData(const Process &p);
  DataBaseID saveReportedProcessData(DataBaseID     reportedHostID,
                                     DataBaseID     procID,
                                     const Process &p);
  DataBaseID saveReferenceURL(const ReferenceURL &url);
  
  DataBaseID saveAlertData(const Alert &a);
  DataBaseID saveMetaAlertData(const MetaAlert &ma);
  
  DataBaseID saveAnalyzerData(const Analyzer &a);
  
  //DataBaseID saveHostData(const HostPtr h);
  DataBaseID saveReportedHostData(DataBaseID  alertID,
                                  DataBaseID  hostID,
				  const std::string role,
                                  const HostPtr h);

  DataBaseID saveServiceData(const Service &s);
  void saveReportedServiceData(DataBaseID  reportedHostID,
                               DataBaseID  serID,
                               const Service &s);

  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntrySaver

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
