/*
 * referenceURL.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_REFERENCEURL_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_REFERENCEURL_HPP_FILE

/* public header */

#include "Persistency/ReferenceURL.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for ReferenceURL.
 */
template<>
struct Comparer<const Persistency::ReferenceURL>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::ReferenceURL &e1, const Persistency::ReferenceURL &e2);
}; // struct Comparer<const Persistency::ReferenceURL>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
