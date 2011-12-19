/*
 * FormatterBuilder.cpp
 *
 */
#include "Preprocessor/Expressions/FormatterBuilder.hpp"


namespace Preprocessor
{
namespace Expressions
{

FormatterBuilder::FormatterBuilder(Formatters::ValuePtrNN value):
  value_(value)
{
}


Formatters::BasePtrNN FormatterBuilder::build(const ConfigIO::Preprocessor::FormatterConfig &cfg)
{
  // TODO
  return value_;
}

} // namespace Expressions
} // namespace Preprocessor
