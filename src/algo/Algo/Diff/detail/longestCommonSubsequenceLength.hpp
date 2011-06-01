/*
 * longestCommonSubsequenceLength.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_LONGESTCOMMONSUBSEQUENCELENGTH_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_LONGESTCOMMONSUBSEQUENCELENGTH_HPP_FILE

#include <string>

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief computes longth of the longest common subsequence out of a given strings.
 *  \param s1 first string.
 *  \param s2 second string.
 *  \return length of the LCS string.
 */
unsigned int longestCommonSubsequenceLength(const std::string &s1, const std::string &s2);

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
