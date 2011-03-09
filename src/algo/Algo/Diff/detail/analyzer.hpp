/*
 * analyzer.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_ANALYZER_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_ANALYZER_HPP_FILE

/* public header */

#include "Persistency/Analyzer.hpp"
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
Similarity compare(const Persistency::Analyzer &e1, const Persistency::Analyzer &e2);

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
