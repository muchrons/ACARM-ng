/*
 * viaPointer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for const pointer.
 */
template<typename T>
struct Comparer<const T*>
{
  /** \brief compares two elements.
   *  \param p1 first element to compare.
   *  \param p2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const T * const p1, const T * const p2);
}; // struct Comparer<const T*>


/** \brief specialization for shared_ptr<>.
 */
template<typename T>
struct Comparer<const boost::shared_ptr<T> >
{
  /** \brief compares two elements.
   *  \param p1 first element to compare.
   *  \param p2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const boost::shared_ptr<T> &p1, const boost::shared_ptr<T> &p2);
}; // struct Comparer<const boost::shared_ptr<T> >


/** \brief specialization for shared_ptr<>.
 */
template<typename T>
struct Comparer<const Commons::SharedPtrNotNULL<T> >
{
  /** \brief compares two elements.
   *  \param p1 first element to compare.
   *  \param p2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Commons::SharedPtrNotNULL<T> &p1, const Commons::SharedPtrNotNULL<T> &p2);
}; // struct Comparer<const Commons::SharedPtrNotNULL<T> >

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
