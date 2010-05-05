/*
 * Cache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CACHE_HPP_FILE

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
class Cache
{
public:

  /** \brief check if element with id is in cache
   *  \param id    id of checked element.
   */
  bool isInCache(DataBaseID id) const
  {
    if(cache_.count(id) > 0)
      return true;
    return false;
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T getFromCache(DataBaseID id) const
  {
    if(cache_.count(id) > 0)
      return cache_.find(id)->second;
    else
      return T();
  }
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T getFromCacheNotNull(DataBaseID id) const
  {
    return cache_.find(id)->second;
  }

  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \param t     added element.
   */
  void addToCache(DataBaseID id, T t)
  {
    std::pair<DataBaseID, T> tmp(id, t);
    cache_.insert(tmp);
  }
private:
  std::map<DataBaseID, T> cache_;
}; // class Cache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
