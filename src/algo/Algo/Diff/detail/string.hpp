/*
 * string.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_STRING_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_STRING_HPP_FILE

/* public header */

#include <string>

#include "Commons/LimitedString.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Algo/Diff/detail/Comparer.hpp"
#include "Algo/Diff/detail/pointerImpl.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for string.
 */
template<>
struct Comparer<const std::string>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const std::string &e1, const std::string &e2);
}; // struct Comparer<const std::string>

/** \brief specialization for C-string.
 */
template<>
struct Comparer<const char *>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const char * const e1, const char * const e2)
  {
    return comparePtrImpl(e1, e2);
  }
}; // struct Comparer<const char *>

/** \brief specialization for LimitedString<>.
 */
template<uint16_t N>
struct Comparer<const Commons::LimitedString<N> >
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Commons::LimitedString<N> &e1, const Commons::LimitedString<N> &e2)
  {
    return compare( e1.get(), e2.get() );
  }
}; // struct Comparer<const Commons::LimitedString<N> >

/** \brief specialization for LimitedNULLString<>.
 */
template<uint16_t N>
struct Comparer<const Commons::LimitedNULLString<N> >
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Commons::LimitedNULLString<N> &e1, const Commons::LimitedNULLString<N> &e2)
  {
    return compare( e1.get(), e2.get() );
  }
}; // struct Comparer<const Commons::LimitedNULLString<N> >

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
