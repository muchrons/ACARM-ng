/*
 * Comparer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_COMPARER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_COMPARER_HPP_FILE

/* public header */

#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief generic template - base for specializations.
 */
template<typename T>
struct Comparer
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   *  \warning generic implementation does not make sens, so it should never be used
   *           thus have no implementation to warn user about problems in compile time.
   */
  static Similarity cmp(const T &/*e1*/, const T &/*e2*/);
}; // struct Comparer

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
