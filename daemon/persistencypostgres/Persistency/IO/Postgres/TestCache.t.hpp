/*
 * TestCache.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCACHE_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCACHE_T_HPP_FILE

#include "Persistency/IO/Postgres/ExceptionNoSuchEntry.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

template<typename S, typename R>
struct TestCache
{
  typedef S Cache;

  template<typename T>
  void fill(T t)
  {
    for(size_t i=0; i<data_.size(); ++i)
    {
      tc_.add(data_[i], t);
      ++t;
    }
  }

  template<typename T>
  bool hasElement(T t)
  {
    try
    {
      tc_.get(t);
      return true;
    }
    catch(const ExceptionNoSuchEntry&)
    {
      return false;
    }
  }
  std::vector<R> data_;
  Cache tc_;
}; // struct TestCache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
