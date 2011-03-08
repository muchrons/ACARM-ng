/*
 * pointerImpl.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_POINTERIMPL_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_POINTERIMPL_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <cassert>
#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{

// forward declaration
template<typename T>
Similarity compare(const T &e1, const T &e2);

namespace detail
{

/** \brief compares two strings - implementation detail.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 *  \note pointers are assumed to be non-NULLs.
 */
template<typename T>
Similarity comparePtrValues(const T * const p1, const T * const p2)
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
template<>
Similarity comparePtrValues<char>(const char * const p1, const char * const p2)
{
  assert(p1!=NULL);
  assert(p2!=NULL);
  return compare( std::string(p1), std::string(p2) );
} // comparePtrValues()


/** \brief compares two strings - implementation detail.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 *  \note pointers are assumed to be non-NULLs.
 */
template<typename T>
Similarity comparePtrImpl(const T * const p1, const T * const p2)
{
  if(p1==p2)
    return 1;
  if(p1==NULL || p2==NULL)
    return 0;

  return comparePtrValues(p1, p2);
} // comparePtrImpl()

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
