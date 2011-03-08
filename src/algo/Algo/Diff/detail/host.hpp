/*
 * host.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_HOST_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_HOST_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for host.
 */
template<>
struct Comparer<const Persistency::Host>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::Host &e1, const Persistency::Host &e2);
}; // struct Comparer<const Persistency::Host>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
