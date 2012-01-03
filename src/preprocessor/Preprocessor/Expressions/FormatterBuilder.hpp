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
  /** \brief exception thrown when invalid number of arguments is given.
   */
  struct ExceptionInvalidNumberOfArguments: public Exception
  {
    /** \brief create object instance.
     *  \param where   location of error detection.
     *  \param func    funciton name.
     *  \param details message details.
     */
    ExceptionInvalidNumberOfArguments(const Location &where, const std::string &func, const char *details);
  }; // struct ExceptionInvalidNumberOfArguments

  /** \brief exception throw when unknown funciton is detected.
   */
  struct ExceptionUnknownFunction: public Exception
  {
    /** \brief creates error message instance.
     *  \param where location of error rising.
     *  \param func  errorneus funciton name.
     */
    ExceptionUnknownFunction(const Location &where, const std::string &func);
  }; // struct ExceptionUnknownFunction

  /** \brief exception thrown when function's argument is invalid.
   *  \param where   location of error rising.
   *  \param func    funciton that error is reported from.
   *  \param details error details.
   */
  struct ExceptionInvalidArgument: public Exception
  {
    /** \brief creates error message instance
     *  \param where   location of error rising.
     *  \param func    funciton that error was found in.
     *  \param details error details.
     */
    ExceptionInvalidArgument(const Location &where, const std::string &func, const char *details);
  }; // struct ExceptionInvalidArgument


  /** \brief creates instance of formatters builder.
   *  \param value object to get value from.
   */
  explicit FormatterBuilder(Formatters::ValuePtrNN value);

  /** \brief builds formatter according to a given config.
   *  \param cfg configuration for controlling building.
   *  \return constructed formatter.
   */
  Formatters::BasePtrNN build(const ConfigIO::Preprocessor::FormatterConfig &cfg) const;

private:
  Formatters::BasePtrNN buildImpl(ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const;
  Formatters::BasePtrNN buildFunction(ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const;

  Formatters::ValuePtrNN value_;
}; // class FormatterBuilder

} // namespace Expressions
} // namespace Preprocessor

#endif
