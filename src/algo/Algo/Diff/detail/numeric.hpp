/*
 * numeric.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_NUMERIC_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_NUMERIC_HPP_FILE

/* public header */

#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
inline Similarity compare(unsigned long e1, unsigned long e2)
{
  return (e1==e2)?1:0;
} // compare()

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
inline Similarity compare(long e1, long e2)
{
  return (e1==e2)?1:0;
} // compare()

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
inline Similarity compare(pid_t e1, pid_t e2)
{
  return (e1==e2)?1:0;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
