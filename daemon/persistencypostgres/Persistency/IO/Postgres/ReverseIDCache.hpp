/*
 * ReverseIDCache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_REVERSEIDCACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_REVERSEIDCACHE_HPP_FILE

#include <map>

#include "Persistency/IO/Postgres/DataBaseID.hpp"

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
  /** \brief type of cache. */
  typedef std::map<DataBaseID, T> RevIDCache;
  /** \brief check if element with id is in cache
   *  \param id    id of checked element.
   */
  bool has(DataBaseID id) const
  {
    return cache_.find(id) != cache_.end();
  }
  // TODO: since T may not be smart_ptr here, consider name like getExisiting() that would throw
  //       in case element would not exist.
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T getNotNull(DataBaseID id) const
  {
    // TODO: SEGV when find() returns cache_.end()
    return cache_.find(id)->second;
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \param t     added element.
   */
  void add(DataBaseID id, T t)
  {
    cache_.insert( std::make_pair<DataBaseID, T>(id, t) );
  }

private:
  RevIDCache cache_;
}; // class Cache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
