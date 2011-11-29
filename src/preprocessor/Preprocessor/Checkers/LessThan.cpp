/*
 * LessThan.cpp
 *
 */
#include "Preprocessor/Checkers/LessThan.hpp"

namespace Preprocessor
{
namespace Checkers
{

LessThan::LessThan(const std::string &str):
  Mode("lessthan"),
  str_(str),
  num_(str_)
{
}

bool LessThan::check(const std::string &str) const
{
  const detail::NumberWrapper p(str);
  // numerical comparison is possible?
  if( num_.isNumber() && p.isNumber() )
    return p.get() < num_.get();
  // if not, fallback to lexicographical
  return str < str_;
}

} // namespace Checkers
} // namespace Preprocessor
