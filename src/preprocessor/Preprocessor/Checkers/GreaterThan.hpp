/*
 * GreaterThan.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_GREATERTHAN_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_GREATERTHAN_HPP_FILE

#include "Preprocessor/Checkers/detail/StrNumCompare.hpp"

namespace Preprocessor
{
namespace Checkers
{

namespace detail
{
struct GreaterThanComparison
{
  template<typename T>
  bool operator()(const T &left, const T &right) const
  {
    return left>right;
  }
}; // struct GreaterThanComparison
} // namespace detail

/** \brief class checking two strings in terms of less-than relation.
 */
class GreaterThan: public detail::StrNumCompare<detail::GreaterThanComparison>
{
public:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  explicit GreaterThan(const std::string &str);
}; // class GreaterThan

} // namespace Checkers
} // namespace Preprocessor

#endif
