/*
 * processTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESSTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESSTERM_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <boost/mpl/at.hpp>

#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

template<typename CIter>
size_t collectionSize(CIter begin, const CIter end)
{
  size_t size=0;
  while(begin!=end)
  {
    ++begin;
    ++size;
  }
  return size;
} // collectionSize()


template<bool isCollection>
struct processCollectionImpl
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    const size_t size=collectionSize(e.begin(), e.end());
    return p.callback().collectionSize(size);
  }
}; // struct processCollectionImpl

template<>
struct processCollectionImpl<false>
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return p.callback().value( Commons::Convert::to<std::string>(e) );
  }
}; // struct processCollectionImpl



template<typename T, typename TParams>
bool processTerm(const T &e, TParams &p)
{
  typedef TParams<ErrorTests>::type ErrH;
  ErrH::throwIfNotEnd(SYSTEM_SAVE_LOCATION, p);
  // process returning size for collection and value for non-collection
  return processCollectionImpl<IsCollection<T>::value>::process(e, p);
} // processTerm()

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
