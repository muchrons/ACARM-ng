/*
 * Equals.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE

#include "Preprocessor/Checkers/detail/StrNumCompare.hpp"

namespace Preprocessor
{
namespace Checkers
{

namespace detail
{
struct EqualityComparison
{
  template<typename T>
  bool operator()(const T &left, const T &right) const
  {
    return left==right;
  }
}; // struct EqualityComparison
} // namespace detail

/** \brief class checking two strings in terms of equality.
 */
class Equals: public detail::StrNumCompare<detail::EqualityComparison>
{
public:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  explicit Equals(const std::string &str);
}; // class Equals

} // namespace Checkers
} // namespace Preprocessor

#endif
