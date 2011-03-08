/*
 * timestamp.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_TIMESTAMP_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_TIMESTAMP_HPP_FILE

/* public header */

#include "Persistency/Timestamp.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for Timestamp.
 */
template<>
struct Comparer<const Persistency::Timestamp>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(Persistency::Timestamp e1, Persistency::Timestamp e2);
}; // struct Comparer<const Persistency::Timestamp>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
