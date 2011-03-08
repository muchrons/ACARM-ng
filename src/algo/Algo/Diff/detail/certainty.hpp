/*
 * certainty.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_CERTAINTY_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_CERTAINTY_HPP_FILE

/* public header */

#include "Persistency/Certainty.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for analyzer.
 */
template<>
struct Comparer<const Persistency::Certainty>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(Persistency::Certainty e1, Persistency::Certainty e2);
}; // struct Comparer<const Persistency::Certainty>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
