/*
 * FunctionParser.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_FUNCTIONPARSER_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_FUNCTIONPARSER_HPP_FILE

#include <string>

#include "ConfigIO/Preprocessor/FormatterConfig.hpp"
#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{
namespace Preprocessor
{

/** \brief parser of function calls, expressed as strings.
 */
class FunctionParser
{
public:
  /** \brief parse required string as a formatter function.
   *  \param formatter string representing function to parse.
   */
  explicit FunctionParser(const std::string &formatter);
  /** \brief gets parsed configuration.
   *  \return parsed configuration.
   */
  const FormatterConfig &getConfig(void) const
  {
    return cfg_;
  }

private:
  FormatterConfig cfg_;
}; // class FunctionParser

} // namespace Preprocessor
} // namespace ConfigIO

#endif
