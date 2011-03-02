/*
 * process.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_PROCESS_HPP_FILE
#define INCLUDE_ALGO_DIFF_PROCESS_HPP_FILE

/* public header */

#include "Persistency/Process.hpp"
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
Similarity compare(const Persistency::Process &e1, const Persistency::Process &e2);

} // namespace Diff
} // namespace Algo

#endif
