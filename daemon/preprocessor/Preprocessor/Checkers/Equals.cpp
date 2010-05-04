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
  Mode("equals"),
  str_(str)
{
}

bool Equals::check(const std::string &str) const
{
  const bool  ret=str_==str;
  const char *msg=(ret?"":"not ");
  LOGMSG_DEBUG_S(log_)<<"'"<<str_<<"' does "<<msg<<"equal '"<<str<<"'";
  return ret;
}

} // namespace Checkers
} // namespace Preprocessor
