/*
 * Contains.cpp
 *
 */
#include "Preprocessor/Checkers/Contains.hpp"

namespace Preprocessor
{
namespace Checkers
{

Contains::Contains(const std::string &str):
  str_(str)
{
}

bool Contains::check(const std::string &str) const
{
  return str.find(str_)!=std::string::npos;
}

} // namespace Checkers
} // namespace Preprocessor
