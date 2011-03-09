/*
 * viaPointer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIAPOINTER_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief compares two elements.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compare(const T *p1, const T *p2);


/** \brief compares two elements.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compare(const boost::shared_ptr<T> &p1, const boost::shared_ptr<T> &p2);


/** \brief compares two elements.
 *  \param p1 first element to compare.
 *  \param p2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compare(const Commons::SharedPtrNotNULL<T> &p1, const Commons::SharedPtrNotNULL<T> &p2);

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
