/*
 * GreaterThan.cpp
 *
 */
#include "Preprocessor/Checkers/GreaterThan.hpp"

namespace Preprocessor
{
namespace Checkers
{

GreaterThan::GreaterThan(const std::string &str):
  detail::StrNumCompare<detail::GreaterThanComparison>("greaterthan", str)
{
}

} // namespace Checkers
} // namespace Preprocessor
