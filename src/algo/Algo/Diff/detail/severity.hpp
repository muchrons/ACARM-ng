/*
 * severity.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_SEVERITY_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_SEVERITY_HPP_FILE

/* public header */

#include "Persistency/Severity.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for Severity.
 */
template<>
struct Comparer<const Persistency::Severity>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(Persistency::Severity e1, Persistency::Severity e2);
}; // struct Comparer<const Persistency::Severity>

/** \brief specialization for SeverityLevel.
 */
template<>
struct Comparer<const Persistency::SeverityLevel>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(Persistency::SeverityLevel e1, Persistency::SeverityLevel e2);
}; // struct Comparer<const Persistency::SeverityLevel>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
