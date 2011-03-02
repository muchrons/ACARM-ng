/*
 * alert.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_ALERT_HPP_FILE
#define INCLUDE_ALGO_DIFF_ALERT_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
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
Similarity compare(const Persistency::Alert &e1, const Persistency::Alert &e2);

} // namespace Diff
} // namespace Algo

#endif
