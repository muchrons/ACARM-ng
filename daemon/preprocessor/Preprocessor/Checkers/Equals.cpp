/*
 * Equals.cpp
 *
 */
#include "Preprocessor/Checkers/Equals.hpp"

namespace Preprocessor
{
namespace Checkers
{

Equals::Equals(const std::string &str):
  str_(str)
{
}

bool Equals::check(const std::string &str) const
{
  return str_==str;
}

} // namespace Checkers
} // namespace Preprocessor
