/*
 * analyzer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_ANALYZER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_ANALYZER_HPP_FILE

/* public header */

#include "Persistency/Analyzer.hpp"
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
struct Comparer<const Persistency::Analyzer>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::Analyzer &e1, const Persistency::Analyzer &e2);
}; // struct Comparer<const Persistency::Analyzer>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
