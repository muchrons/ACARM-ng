/*
 * Equals.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Preprocessor/Checkers/Equals.hpp"

namespace Preprocessor
{
namespace Checkers
{

Equals::Equals(const std::string &str):
  detail::StrNumCompare<detail::EqualityComparison>("equals", str)
{
}

} // namespace Checkers
} // namespace Preprocessor
