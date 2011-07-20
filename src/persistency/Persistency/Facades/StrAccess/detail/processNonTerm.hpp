/*
 * processNonTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESSNONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESSNONTERM_HPP_FILE

/* public header */

#include <cstdlib>
#include <boost/mpl/at.hpp>

#include "Persistency/Facades/StrAccess/Params.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

template<typename T, typename TParams>
bool processNonTerm(const T &e, TParams &p)
{
  // TODO: add handling of collections and pointers
  //return boost::mpl::at<THandleMap, T>::type::get(e, p);
  return TParams::handle<T>::type::get(e, p);
} // processNonTerm()

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
