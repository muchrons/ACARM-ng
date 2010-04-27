/*
 * CheckContains.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKCONTAINS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKCONTAINS_HPP_FILE

#include "Preprocessor/CheckMode.hpp"

// TODO: comment

namespace Preprocessor
{

class CheckContains: public CheckMode
{
public:
  explicit CheckContains(const std::string &str);

  virtual bool check(const std::string &str) const;

public:
  const std::string str_;
}; // class CheckContains

} // namespace Preprocessor

#endif
