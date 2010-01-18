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
  // TODO


private:
  DataBaseID getID(const std::string &seqName);

  DataBaseID saveProcessData(const Process &p);
  DataBaseID saveReportedProcessData(DataBaseID     reportedHostID,
                                     DataBaseID     procID,
                                     const Process &p);
  DataBaseID saveReferenceURL(const ReferenceURL &url);

  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntrySaver

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
