/*
 * Equals.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"

// TODO: comment

namespace Preprocessor
{
namespace Checkers
{

class Equals: public Mode
{
public:
  explicit Equals(const std::string &str);

  virtual bool check(const std::string &str) const;

public:
  const std::string str_;
}; // class Equals

} // namespace Checkers
} // namespace Preprocessor

#endif
