/*
 * alert.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_ALERT_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_ALERT_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for alert.
 */
template<>
struct Comparer<const Persistency::Alert>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::Alert &e1, const Persistency::Alert &e2);
}; // struct Comparer<const Persistency::Alert>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
