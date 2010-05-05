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

/** \brief cache for .
 */
template<typename T>
class Cache
{
public:

  bool isInCache(DataBaseID id) const
  {
    if(cache_.count(id) > 0)
      return true;
    return false;
  }
  T getFromCache(DataBaseID id) const
  {
    if(cache_.count(id) > 0)
      return cache_.find(id)->second;
    else
      return T();
  }
  T getFromCacheNotNull(DataBaseID id) const
  {
    return cache_.find(id)->second;
  }

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
