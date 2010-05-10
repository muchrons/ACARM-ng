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

// TODO: test for this class are missing.

// TODO: there is already IDCache class so name 'Cache' does not give a clue
//       about this class' semantics. consider renaming it to something like
//       ReverseIDCache.

/** \brief cache which stores elements with IDs.
 */
template<typename T>
class Cache
{
public:
  // TODO: Cache is already in class name - consider simple has() method; see IDCache for example.
  /** \brief check if element with id is in cache
   *  \param id    id of checked element.
   */
  bool isInCache(DataBaseID id) const
  {
    // TODO: use find() instead of count() - find is O(lg(n)), while count() is O(n) (with high constant part, in practice)
    // TODO: return <condition>; is fine here
    if(cache_.count(id) > 0)
      return true;
    return false;
  }
  // TODO: Cache is already in class name - consider simple get() method; see IDCache for example.
  // TODO: since you already have has() and getNotNull() methods, consider rmoving this method at all -
  //       it is redundant and has a bit problematic API (see notes below).
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T getFromCache(DataBaseID id) const
  {
    // TODO: use find() here - if it will not find element, end() will be returned.
    if(cache_.count(id) > 0)
      return cache_.find(id)->second;
    else
      // TODO: enforcing default c-tor on class creator is not good idea in general and should
      //       be avoided. consider returning pointer to element held inside.
      return T();
  }
  // TODO: Cache is already in class name - consider simple getNotNull() method; see IDCache for example.
  // TODO: since T may not be smart_ptr here, consider name like getExisiting() that would throw
  //       in case element would not exist.
  /** \brief get element with id from cache
   *  \param id    id of element.
   */
  T getFromCacheNotNull(DataBaseID id) const
  {
    // TODO: SEGV when find() returns cache_.end()
    return cache_.find(id)->second;
  }
  // TODO: Cache is already in class name - consider simple add() method; see IDCache for example.
  /** \brief get element with id from cache
   *  \param id    id of element.
   *  \param t     added element.
   */
  void addToCache(DataBaseID id, T t)
  {
    // TODO: this variable should be const
    std::pair<DataBaseID, T> tmp(id, t);
    // TODO: consider making unnamed variable here (ex. with make_pair<>() template)
    cache_.insert(tmp);
  }

private:
  // TODO: make this type typedef - it will be useful when declaring iterators to be used with
  //       map<>::find() method.
  std::map<DataBaseID, T> cache_;
}; // class Cache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
