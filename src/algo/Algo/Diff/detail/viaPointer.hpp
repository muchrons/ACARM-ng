/*
 * viaPointer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <cstring>
#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/Diff/detail/Comparer.hpp"
#include "Algo/Diff/compare.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

// forward declaration required for dependency
Similarity compare(const std::string &e1, const std::string &e2);

namespace detail
{
/** \brief compares two strings - implementation detail.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 *  \note pointers are assumed to be non-NULLs.
 */
template<typename T>
Similarity comparePtrValues(const T *p1, const T *p2)
{
  assert(p1!=NULL);
  assert(p2!=NULL);
  return compare(*p1, *p2);
} // comparePtrValues()

/** \brief compares two strings - implementation detail.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 *  \note pointers are assumed to be non-NULLs.
 */
inline Similarity comparePtrValues(const char *p1, const char *p2)
{
  assert(p1!=NULL);
  assert(p2!=NULL);
  return compare( std::string(p1), std::string(p2) );
} // comparePtrValues()
} // namesace detail


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
  static Similarity cmp(const T *p1, const T *p2)
  {
    if(p1==p2)
      return 1;
    if(p1==NULL || p2==NULL)
      return 0;

    return detail::comparePtrValues(p1, p2);
  }
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
  static Similarity cmp(const boost::shared_ptr<T> &p1, const boost::shared_ptr<T> &p2)
  {
    return compare( p1.get(), p2.get() );
  }
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
  static Similarity cmp(const Commons::SharedPtrNotNULL<T> &p1, const Commons::SharedPtrNotNULL<T> &p2)
  {
    return compare( p1.get(), p2.get() );
  }
}; // struct Comparer<const Commons::SharedPtrNotNULL<T> >

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
