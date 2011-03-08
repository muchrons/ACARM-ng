/*
 * portNumber.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_PORTNUMBER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_PORTNUMBER_HPP_FILE

/* public header */

#include "Persistency/PortNumber.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for PortNumber.
 */
template<>
struct Comparer<const Persistency::PortNumber>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::PortNumber e1, const Persistency::PortNumber e2)
  {
    return ( e1.get()==e2.get() )?1:0;
  }
}; // struct Comparer<const Persistency::PortNumber>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
