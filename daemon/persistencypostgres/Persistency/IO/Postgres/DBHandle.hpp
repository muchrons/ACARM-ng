/*
 * DBHandle.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DBHANDLE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DBHANDLE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IO/Postgres/DBConnection.hpp"
#include "Persistency/IO/Postgres/IDCache.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief class reprenseting all data required to work with data base.
 */
class DBHandle: private boost::noncopyable
{
public:
  /** \brief create data base handle.
   *  \param connParams parameters for DBConnection object.
   *  \param idCache    cache for ID in data base.
   */
  DBHandle(const DBConnection::Parameters &connParams,
           IDCachePtrNN                    idCache):
    conn_(connParams),
    idCache_(idCache)
  {
  }
  /** \brief gets connection object.
   *  \return data base conneciton object's reference.
   */
  DBConnection &getConnection(void)
  {
    return conn_;
  }
  /** \brief gets cache of IDs.
   *  \return pointer to ID cache.
   */
  IDCachePtrNN getIDCache(void)
  {
    return idCache_;
  }

private:
  DBConnection conn_;
  IDCachePtrNN idCache_;
}; // class DBHandle



typedef Commons::SharedPtrNotNULL<DBHandle> DBHandlePtrNN;

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
