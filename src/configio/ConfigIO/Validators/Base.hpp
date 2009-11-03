/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_VALIDATORS_BASE_HPP_FILE
#define INCLUDE_CONFIGIO_VALIDATORS_BASE_HPP_FILE

#include <string>

#include "ConfigIO/LoggerAppenderConfig.hpp"


namespace ConfigIO
{
namespace Validators
{

struct Base
{
  typedef LoggerAppenderConfig::Options AppOptions;

  virtual ~Base(void)
  {
  }

  virtual void validate(const AppOptions  &opts) const = 0;

  virtual const char *typeToValidate(void) const = 0;
}; // class ConsoleAppender

} // namespace Validators
} // namespace ConfigIO

#endif
