/*
 * Value.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/Value.hpp"
#include "System/ignore.hpp"
#include "Commons/Convert.hpp"


namespace Preprocessor
{
namespace Formatters
{

Value::Value(void):
  Base("value", Formatters())
{
}

void Value::set(const double d)
{
  set( Commons::Convert::to<std::string>(d) );
}

std::string Value::execImpl(const Arguments &args) const
{
  assert(args.size()==0);
  System::ignore(args);
  return str_;
}

} // namespace Formatters
} // namespace Preprocessor
