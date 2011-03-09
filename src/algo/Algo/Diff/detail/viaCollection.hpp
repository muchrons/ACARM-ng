/*
 * viaCollection.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTION_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "Base/NonEmptyVector.hpp"
#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compare(const std::vector<T> &e1, const std::vector<T> &e2);

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compare(const Base::NonEmptyVector<T> &e1, const Base::NonEmptyVector<T> &e2);

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
