/*
 * Contains.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Preprocessor/Checkers/Contains.hpp"

namespace Preprocessor
{
namespace Checkers
{

Contains::Contains(const std::string &str):
  Mode("contains"),
  str_(str)
{
}

bool Contains::check(const std::string &str) const
{
  const bool  ret=str.find(str_)!=std::string::npos;
  const char *msg=(ret?"":"not ");
  LOGMSG_DEBUG_S(log_)<<"'"<<str_<<"' does "<<msg<<"contain '"<<str<<"'";
  return ret;
}

} // namespace Checkers
} // namespace Preprocessor
