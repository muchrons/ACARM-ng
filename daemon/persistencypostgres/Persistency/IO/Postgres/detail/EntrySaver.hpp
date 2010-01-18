/*
 * EntrySaver.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_HPP_FILE

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

class EntrySaver: private boost::noncopyable
{
public:
  EntrySaver(Transaction &t, DBHandler &dbh);

  DataBaseID saveProcess(DataBaseID reportedHostID, const Process &p);
  // TODO

  DataBaseID getID(const std::string &seqName);

private:
  DBHandler   &dbh_;
  Transaction &t_;
}; // class EntrySaver

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
