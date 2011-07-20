/*
 * process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_PROCESS_HPP_FILE

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

template<typename THandleMap, typename T>
void process(const T &e, Params &p)
{
  // TODO: add handling of collections and pointers
  boost::mpl::at<THandleMap, T>::type::get(e, p);
}

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
