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

  //Alert &readAlert(DataDaseID alertID);

  //Alert::SourceAnalyzers &getAnalyzers(DataBaseID alertID);
private:
  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntryReader

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency


#endif
