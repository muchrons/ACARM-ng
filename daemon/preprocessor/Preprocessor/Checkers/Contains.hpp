/*
 * Contains.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_CONTAINS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_CONTAINS_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"

// TODO: comment

namespace Preprocessor
{
namespace Checkers
{

class Contains: public Mode
{
public:
  explicit Contains(const std::string &str);

  virtual bool check(const std::string &str) const;

public:
  const std::string str_;
}; // class Contains

} // namespace Checkers
} // namespace Preprocessor

#endif
