/*
 * Proxy.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_PROXY_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_PROXY_HPP_FILE

/* public header */

#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief helper proxy object.
 */
template<typename T>
struct Proxy
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const T &e1, const T &e2)
  {
    return Comparer<const T>::cmp(e1, e2);
  }
}; // struct Proxy

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
