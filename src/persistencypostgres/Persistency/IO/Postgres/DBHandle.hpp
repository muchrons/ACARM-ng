/*
 * DBHandle.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DBHANDLE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DBHANDLE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/setThreadName.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Persistency/IO/Postgres/DBConnection.hpp"
#include "Persistency/IO/Postgres/IDCache.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

// TODO: add thread for periodically IDCache cleanup

namespace
{
struct IDCachePruneThread
{
  explicit IDCachePruneThread(IDCachePtrNN  idCache):
    log_("persistency.io.postgres.idcacheprunethread"),
    idCache_(idCache)
  {
    Commons::setThreadName("id_prunethread");
  }

  void operator()(void)
  {
    LOGMSG_DEBUG(log_, "IDCache prune thread started");

    bool quit=false;
    // prune IDCache every 10[s]
    while(!quit)
    {
      try
      {
        // wait a while before next polling (NOTE: this try{}catch MUST start with
        // interruptable event, in case pinging or discarding messages would throw).
        boost::this_thread::sleep( boost::posix_time::seconds(10) );
        idCache_->prune();
        // TODO
      }
      catch(const boost::thread_interrupted &)
      {
        // ok - thread has been interrupted
        LOGMSG_DEBUG(log_, "interruption requested - exiting");
        quit=true;
      }
      catch(const std::exception &ex)
      {
        // hmmm...
        LOGMSG_WARN_S(log_)<<"exception caught: "<<ex.what();
      }
    } // while(true)

    LOGMSG_DEBUG(log_, "IDCache prune thread terminated");
  }

private:
  Logger::Node  log_;
  IDCachePtrNN  idCache_;
}; // struct IDCachePruneThread
} // unnamed nmespace

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
    idCache_(idCache),
    pruneThread_( IDCachePruneThread(idCache) )
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
  DBConnection             conn_;
  IDCachePtrNN             idCache_;
  Commons::Threads::Thread pruneThread_;
}; // class DBHandle



typedef Commons::SharedPtrNotNULL<DBHandle> DBHandlePtrNN;

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
