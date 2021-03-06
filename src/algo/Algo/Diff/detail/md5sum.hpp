/*
 * md5sum.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_MD5SUM_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_MD5SUM_HPP_FILE

/* public header */

#include "Persistency/MD5Sum.hpp"
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
Similarity compare(const Persistency::MD5Sum &e1, const Persistency::MD5Sum &e2);

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
