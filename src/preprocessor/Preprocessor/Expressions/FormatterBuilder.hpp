/*
 * FormatterBuilder.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_FORMATTERBUILDER_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_FORMATTERBUILDER_HPP_FILE

#include "ConfigIO/Preprocessor/FormatterConfig.hpp"
#include "Preprocessor/Formatters/Base.hpp"
#include "Preprocessor/Formatters/Value.hpp"


namespace Preprocessor
{
namespace Expressions
{

/** \brief builder of formatter object, basing on a given configuration.
 */
class FormatterBuilder
{
public:
  explicit FormatterBuilder(Formatters::ValuePtrNN value);

  Formatters::BasePtrNN build(const ConfigIO::Preprocessor::FormatterConfig &cfg);

private:
  Formatters::ValuePtrNN value_;
}; // class FormatterBuilder

} // namespace Expressions
} // namespace Preprocessor

#endif
