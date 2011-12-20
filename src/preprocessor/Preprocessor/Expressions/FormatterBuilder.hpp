/*
 * FormatterBuilder.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_FORMATTERBUILDER_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_FORMATTERBUILDER_HPP_FILE

#include "ConfigIO/Preprocessor/FormatterConfig.hpp"
#include "Preprocessor/Exception.hpp"
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
  struct ExceptionInvalidNumberOfArguments: public Exception
  {
    ExceptionInvalidNumberOfArguments(const Location &where, const std::string &func, const char *details);
  }; // struct ExceptionInvalidNumberOfArguments

  struct ExceptionUnknownFunction: public Exception
  {
    ExceptionUnknownFunction(const Location &where, const std::string &func);
  }; // struct ExceptionUnknownFunction

  struct ExceptionInvalidArgument: public Exception
  {
    ExceptionInvalidArgument(const Location &where, const std::string &func, const char *details);
  }; // struct ExceptionInvalidArgument


  explicit FormatterBuilder(Formatters::ValuePtrNN value);

  Formatters::BasePtrNN build(const ConfigIO::Preprocessor::FormatterConfig &cfg) const;

private:
  Formatters::BasePtrNN buildImpl(ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const;
  Formatters::BasePtrNN buildFunction(ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const;

  Formatters::ValuePtrNN value_;
}; // class FormatterBuilder

} // namespace Expressions
} // namespace Preprocessor

#endif
