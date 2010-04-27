/*
 * CheckEquals.cpp
 *
 */
#include "Preprocessor/CheckEquals.hpp"

namespace Preprocessor
{

CheckEquals::CheckEquals(const std::string &str):
  str_(str)
{
}

bool CheckEquals::check(const std::string &str) const
{
  return str_==str;
}

} // namespace Preprocessor
