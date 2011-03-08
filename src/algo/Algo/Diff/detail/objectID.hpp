/*
 * objectID.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_OBJECTID_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_OBJECTID_HPP_FILE

/* public header */

#include <string>

#include "Base/ObjectID.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for ObjectID.
 */
template<typename T>
struct Comparer<const Base::ObjectID<T> >
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Base::ObjectID<T> e1, const Base::ObjectID<T> e2)
  {
    return ( e1.get()==e2.get() )?1.0:0.0;
  }
}; // struct Comparer<const Base::ObjectID<T> >

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
