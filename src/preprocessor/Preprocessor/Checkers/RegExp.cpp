/*
 * RegExp.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Preprocessor/Checkers/RegExp.hpp"


namespace Preprocessor
{
namespace Checkers
{

RegExp::RegExp(const std::string &expr):
  Mode("regexp"),
  re_(expr, boost::regex::extended)
{
  LOGMSG_INFO_S(log_)<<"created comparer for regular expression: '"<<expr<<"'";
}

bool RegExp::check(const std::string &str) const
{
  return boost::regex_search(str, re_, boost::regex_constants::match_partial);
}

} // namespace Checkers
} // namespace Preprocessor
