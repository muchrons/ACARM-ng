/*
 * ReverseIDCache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_REVERSEIDCACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_REVERSEIDCACHE_HPP_FILE

#include <map>

#include "Persistency/IO/Postgres/DataBaseID.hpp"
#include "Persistency/IO/Postgres/ExceptionNoSuchEntry.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief cache which stores elements with IDs.
 */
template<typename T>
class ReverseIDCache
{
public:
  // TODO: this typedef should be private
  /** \brief type of cache. */
  typedef std::map<DataBaseID, T> RevIDCache;

  /** \brief check if element with id is in cache
   *  \param id    id of checked element.
   */
  bool has(DataBaseID id) const
  {
    return cache_.find(id) != cache_.end();
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T get(DataBaseID id) const
  {
    // TODO: use RevIDCache typedef for name here
    typename std::map<DataBaseID, T>::const_iterator it = cache_.find(id);
    if( it == cache_.end() )
      throw ExceptionNoSuchEntry(SYSTEM_SAVE_LOCATION);
    return it->second;
  }
  // TODO: 't' can be taken by const-ref here
  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \param t     added element.
   */
  void add(DataBaseID id, T t)
  {
    // TODO: make_pair() should be able to deduce parameters by itself in this context,
    //       therefore specifying them explicitly can be ommited.
    // TODO: best solution would be to use RevIDCache::value_type type here
    cache_.insert( std::make_pair<DataBaseID, T>(id, t) );
  }

private:
  RevIDCache cache_;
}; // class Cache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
