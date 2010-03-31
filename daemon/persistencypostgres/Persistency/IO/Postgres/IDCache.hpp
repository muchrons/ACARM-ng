/*
 * IDCache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_IDCACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_IDCACHE_HPP_FILE

#include <boost/noncopyable.hpp>
#include <cassert>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Postgres/StorageDataCache.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief common cache for all types of data-base entries.
 *
 *  class given unified entry point to ID caches for different object types.
 *  class is thread-safe.
 */
class IDCache: private boost::noncopyable
{
public:
  /** \brief gets ID for a given host.
   *  \param host host to get ID for.
   *  \return data-base ID for a given host.
   */
  DataBaseID get(HostPtrNN host)
  {
    return hostsIDs_.get(host);
  }
  /** \brief adds new entry to cache.
   *  \param host host to be added.
   *  \param id   data base ID to associate with a given host.
   */
  void add(HostPtrNN host, DataBaseID id)
  {
    return hostsIDs_.add(host, id);
  }

  /** \brief gets ID for a given alert.
   *  \param alert alert to get ID for.
   *  \return data-base ID or a given alert.
   */
  DataBaseID get(AlertPtrNN alert)
  {
    return alertsIDs_.get(alert);
  }
  /** \brief adds new entry to cache.
   *  \param alert alert to be added.
   *  \param id    id to associate with given alert.
   */
  void add(AlertPtrNN alert, DataBaseID id)
  {
    return alertsIDs_.add(alert, id);
  }

  /** \brief gets ID for a given meta-alert.
   *  \param ma meta-alert to get ID for.
   *  \return data-base ID or a given meta-alert.
   */
  DataBaseID get(MetaAlertPtrNN ma)
  {
    return metaAlertsIDs_.get(ma);
  }
  /** \brief adds new entry to cache.
   *  \param ma meta-alert to be added.
   *  \param id id to associate with given meta-alert.
   */
  void add(MetaAlertPtrNN ma, DataBaseID id)
  {
    return metaAlertsIDs_.add(ma, id);
  }

  /** \brief removes entries that are no longer prenset in the memory.
   */
  void prune(void);

  /** \brief check if alert is in cache
   *  \param alert alert to be checked
   *  \return true if alert is in cache
   */
  bool has(AlertPtrNN alert)
  {
    return alertsIDs_.hasImpl(alert);
  }

  /** \brief check if meta-alert is in cache
   *  \param ma meta-alert to be checked
   *  \return true if alert is in cache
   */
  bool has(MetaAlertPtrNN ma)
  {
    return metaAlertsIDs_.hasImpl(ma);
  }


  /** \brief check if host is in cache
   *  \param host host to be checked
   *  \return true if host is in cache
   */
  bool has(HostPtrNN host)
  {
    return hostsIDs_.hasImpl(host);
  }
private:
  StorageDataCache<Persistency::Host>      hostsIDs_;
  StorageDataCache<Persistency::Alert>     alertsIDs_;
  StorageDataCache<Persistency::MetaAlert> metaAlertsIDs_;
}; // class IDCache


/** \brief non-NULL, shared pointer to IDCache. */
typedef Commons::SharedPtrNotNULL<IDCache> IDCachePtrNN;

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
