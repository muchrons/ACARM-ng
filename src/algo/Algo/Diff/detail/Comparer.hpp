/*
 * Comparer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_COMPARER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_COMPARER_HPP_FILE

/* public header */

#include <boost/static_assert.hpp>

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
   *           thus have invalid implementation to warn user about problems in compile time.
   */
  static Similarity cmp(const T &/*e1*/, const T &/*e2*/)
  {
    // this does not make sense
    BOOST_STATIC_ASSERT( sizeof(T) && false );
    return Similarity(42);  // code never reaches here
  }
}; // struct Comparer

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
