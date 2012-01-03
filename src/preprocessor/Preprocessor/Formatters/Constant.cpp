/*
 * Constant.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/Constant.hpp"
#include "System/ignore.hpp"


namespace Preprocessor
{
namespace Formatters
{

Constant::Constant(const std::string &str):
  Base("constant", Formatters()),
  str_(str)
{
  LOGMSG_DEBUG_S(log_)<<"setting constant value to '"<<str_<<"'";
}

std::string Constant::execImpl(const Arguments &args) const
{
  assert(args.size()==0);
  System::ignore(args);
  return str_;
}

} // namespace Formatters
} // namespace Preprocessor
