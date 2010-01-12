/*
 * EntrySaver.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

using namespace std;
using namespace pqxx;

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

EntrySaver::EntrySaver(Transaction &t, DBHandler &dbh):
  t_( t.getAPI<Postgres::TransactionAPI>().getHandle() ),
  dbh_(dbh)
{
}

DataBaseID EntrySaver::saveProcess(DataBaseID reportedHostID, const Process &p)
{
  // write data to DB
  stringstream ss;
  ss << "INSERT INTO procs(path, name, md5) VALUES (";
  ss << "'" << pqxx::sqlesc( p.getPath().get() ) << "',";
  ss << "'" << pqxx::sqlesc( p.getName().get() ) << "',";

  if( p.getMD5()!=NULL )
    ss << "'" << pqxx::sqlesc( p.getMD5()->get() ) << "'";
  else
    ss << "NULL";

  ss << ");";
  t_.exec(ss);
  // TODO

  // get the ID
  return getID("procs_id_seq");
}

DataBaseID EntrySaver::getID(const std::string &seqName)
{
  assert( seqName==pqxx::sqlesc(seqName) && "invalid sequence name" );

  result r=t_.exec( "SELECT currval('" + seqName + "') as id;" );
  assert( r.size()==1 && "unable to read current sequence number" );

  DataBaseID id;
  r[0]["id"].to(id);
  return id;
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
