/*
 * severity.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_TIMESTAMP_HPP_FILE
#define INCLUDE_ALGO_DIFF_TIMESTAMP_HPP_FILE

/* public header */

#include "Persistency/Severity.hpp"
#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
Similarity compare(Persistency::Severity e1, Persistency::Severity e2);

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
Similarity compare(Persistency::SeverityLevel e1, Persistency::SeverityLevel e2);

} // namespace Diff
} // namespace Algo

#endif
