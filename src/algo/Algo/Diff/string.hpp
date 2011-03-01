/*
 * string.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_STRING_HPP_FILE
#define INCLUDE_ALGO_DIFF_STRING_HPP_FILE

/* public header */

#include <string>

#include "Commons/LimitedString.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Algo/Diff/Similarity.hpp"
#include "Algo/Diff/viaPointer.hpp"

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

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
template<uint16_t N>
Similarity compare(const Commons::LimitedString<N> &e1, const Commons::LimitedString<N> &e2)
{
  return compare( e1.get(), e2.get() );
}

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
template<uint16_t N>
Similarity compare(const Commons::LimitedNULLString<N> &e1, const Commons::LimitedNULLString<N> &e2)
{
  return compare( e1.get(), e2.get() );
}

} // namespace Diff
} // namespace Algo

#endif
