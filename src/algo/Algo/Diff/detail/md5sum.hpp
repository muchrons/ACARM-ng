/*
 * md5sum.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_MD5SUM_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_MD5SUM_HPP_FILE

/* public header */

#include "Persistency/MD5Sum.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for MD5Sum.
 */
template<>
struct Comparer<const Persistency::MD5Sum>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::MD5Sum &e1, const Persistency::MD5Sum &e2);
}; // struct Comparer<const Persistency::MD5Sum>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
