/*
 * proxy.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_PROXY_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_PROXY_HPP_FILE

/* public header */

#include "Algo/Diff/detail/decl.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief helper proxy object.
 */
template<typename T>
Similarity proxy(const T &e1, const T &e2)
{
  return Comparer<const T>::cmp(e1, e2);
} // proxy()

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
