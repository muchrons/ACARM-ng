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
private:
  /** \brief type of cache. */
  typedef std::map<DataBaseID, T> RevIDCache;
public:
  /** \brief check if element with id is in cache
   *  \param id    id of checked element.
   *  \return return true if element with given ID is in cache. // TODO: when returning true/false sentence must always end with 'false/true otherwise' to clear all doubts. for example consider bool has(...) { return true; } - it would be consistent with provided description.
   */
  bool has(DataBaseID id) const
  {
    return cache_.find(id) != cache_.end();
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \return return entry with given ID.
   */
  T get(DataBaseID id) const
  {
    typename RevIDCache::const_iterator it = cache_.find(id);
    if( it == cache_.end() )
      throw ExceptionNoSuchEntry(SYSTEM_SAVE_LOCATION);
    return it->second;
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \param t     added element.
   */
  void add(DataBaseID id, const T &t)
  {
    cache_.insert( typename RevIDCache::value_type(id, t) );
  }

private:
  RevIDCache cache_;
}; // class Cache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
