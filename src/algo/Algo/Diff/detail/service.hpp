/*
 * service.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_SERVICE_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_SERVICE_HPP_FILE

/* public header */

#include "Persistency/Service.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for Service.
 */
template<>
struct Comparer<const Persistency::Service>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Persistency::Service &e1, const Persistency::Service &e2);
}; // struct Comparer<const Persistency::Service>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
