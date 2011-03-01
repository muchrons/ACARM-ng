/*
 * string.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_STRING_HPP_FILE
#define INCLUDE_ALGO_DIFF_STRING_HPP_FILE

/* public header */

#include <string>

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
Similarity compare(const std::string &e1, const std::string &e2);

} // namespace Diff
} // namespace Algo

#endif
