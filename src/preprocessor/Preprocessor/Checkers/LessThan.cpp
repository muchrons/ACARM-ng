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
  detail::StrNumCompare<detail::LessThanComparison>("lessthan", str)
{
}

} // namespace Checkers
} // namespace Preprocessor
