/*
 * NumberWrapper.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_NUMBERWRAPPER_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_NUMBERWRAPPER_HPP_FILE

#include <string>
#include <cassert>

namespace Preprocessor
{
namespace Checkers
{

/** \brief class being wrapper for numeric types, represented as strings.
 */
class NumberWrapper
{
public:
  explicit NumberWrapper(const std::string &str);

  bool isNumber(void) const
  {
    return isNum_;
  }
  double get(void) const
  {
    assert( isNumber() && "getting value of non-number is undefined (logical error)" );
    return value_;
  }

private:
  bool isNumber(const std::string &str) const;

  bool   isNum_;
  double value_;
}; // class NumberWrapper

} // namespace Checkers
} // namespace Preprocessor

#endif
