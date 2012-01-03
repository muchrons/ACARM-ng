/*
 * LessThan.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_LESSTHAN_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_LESSTHAN_HPP_FILE

#include "Preprocessor/Checkers/detail/StrNumCompare.hpp"

namespace Preprocessor
{
namespace Checkers
{

namespace detail
{
/** \brief comparison implementaiton object.
 */
struct LessThanComparison
{
  /** \brief compares arguments for less-than relation.
   *  \param left  left-hand argument.
   *  \param right right-hand argument.
   *  \return left<right.
   */
  template<typename T>
  bool operator()(const T &left, const T &right) const
  {
    return left<right;
  }
}; // struct LessThanComparison
} // namespace detail

/** \brief class checking two strings in terms of less-than relation.
 */
class LessThan: public detail::StrNumCompare<detail::LessThanComparison>
{
public:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  explicit LessThan(const std::string &str);
}; // class LessThan

} // namespace Checkers
} // namespace Preprocessor

#endif
