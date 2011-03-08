/*
 * process.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_PROCESS_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_PROCESS_HPP_FILE

/* public header */

#include "Persistency/Process.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for Process.
 */
template<>
struct Comparer<const Persistency::Process>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::Process &e1, const Persistency::Process &e2);
}; // struct Comparer<const Persistency::Process>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
