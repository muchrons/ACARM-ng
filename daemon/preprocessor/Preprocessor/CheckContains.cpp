/*
 * CheckContains.cpp
 *
 */
#include "Preprocessor/CheckContains.hpp"

namespace Preprocessor
{

CheckContains::CheckContains(const std::string &str):
  str_(str)
{
}

bool CheckContains::check(const std::string &str) const
{
  return str.find(str_)!=std::string::npos;
}

} // namespace Preprocessor
