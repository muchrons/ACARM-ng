/*
 * Value.cpp
 *
 */
#include <cassert>

#include "System/ignore.hpp"
#include "Preprocessor/Formatters/Value.hpp"


namespace Preprocessor
{
namespace Formatters
{

Value::Value(void):
  Base("value", Formatters())
{
}

std::string Value::execImpl(const Arguments &args) const
{
  assert(args.size()==0);
  System::ignore(args);
  return str_;
}

} // namespace Formatters
} // namespace Preprocessor
