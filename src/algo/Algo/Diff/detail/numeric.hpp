/*
 * numeric.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_NUMERIC_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_NUMERIC_HPP_FILE

/* public header */

#include "Algo/Diff/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for numeric.
 */
template<>
struct Comparer<const unsigned long>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(unsigned long e1, unsigned long e2)
  {
    return (e1==e2)?1:0;
  }
}; // struct Comparer<const unsigned long>


/** \brief specialization for numeric.
 */
template<>
struct Comparer<const long>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(long e1, long e2)
  {
    return (e1==e2)?1:0;
  }
}; // struct Comparer<const long>


/** \brief specialization for numeric.
 */
template<>
struct Comparer<const pid_t>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(pid_t e1, pid_t e2)
  {
    return (e1==e2)?1:0;
  }
}; // struct Comparer<const pid_t>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
