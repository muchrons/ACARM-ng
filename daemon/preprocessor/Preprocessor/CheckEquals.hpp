/*
 * CheckEquals.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKEQUALS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKEQUALS_HPP_FILE

#include "Preprocessor/CheckMode.hpp"

// TODO: comment

namespace Preprocessor
{

class CheckEquals: public CheckMode
{
public:
  explicit CheckEquals(const std::string &str);

  virtual bool check(const std::string &str) const;

public:
  const std::string str_;
}; // class CheckEquals

} // namespace Preprocessor

#endif
