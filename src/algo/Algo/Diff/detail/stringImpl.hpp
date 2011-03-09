/*
 * stringImpl.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_STRINGIMPL_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_STRINGIMPL_HPP_FILE

/* public header */

#include "Algo/Diff/detail/string.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

template<uint16_t N>
inline Similarity compare(const Commons::LimitedString<N> &e1, const Commons::LimitedString<N> &e2)
{
  return compare( e1.get(), e2.get() );
}

template<uint16_t N>
inline Similarity compare(const Commons::LimitedNULLString<N> &e1, const Commons::LimitedNULLString<N> &e2)
{
  return compare( e1.get(), e2.get() );
}

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
