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
  /** \brief gets ID for a given Host/Alert/MetaAlert.
   *  \param t Host/Alert/MetaAlert to get ID for.
   *  \return data-base ID for a given Host/Alert/MetaAlert.
   */
  template<typename T>
  DataBaseID get(const T &t)
  {
    return getDataHolder(t).get(t);
  }

  /** \brief adds new entry to cache.
   *  \param t  Host/Alert/MetaAlerti to be added.
   *  \param id ID to associate with given Host/Alert/MetaAlert.
   */
  template<typename T>
  void add(const T &t, DataBaseID id)
  {
    return getDataHolder(t).add(t, id);
  }

  /** \brief check if Host/Alert/MetaAlert is in cache
   *  \param t Host/Alert/MetaAlert to be checked
   *  \return true if Host/Alert/MetaAlert is in cache
   */
  template<typename T>
  bool has(const T &t)
  {
    return getDataHolder(t).has(t);
  }

  /** \brief removes entries that are no longer prenset in the memory.
   */
  void prune(void);

private:

  StorageDataCache<Persistency::Host> &getDataHolder(const HostPtrNN &)
  {
    return hostsIDs_;
  }

  StorageDataCache<Persistency::Alert> &getDataHolder(const AlertPtrNN &)
  {
    return alertsIDs_;
  }

  StorageDataCache<Persistency::MetaAlert> &getDataHolder(const MetaAlertPtrNN &)
  {
    return metaAlertsIDs_;
  }

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
