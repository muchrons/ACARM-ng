/*
 * ConsoleAppender.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_VALIDATORS_CONSOLEAPPENDER_HPP_FILE
#define INCLUDE_CONFIGIO_VALIDATORS_CONSOLEAPPENDER_HPP_FILE

#include "ConfigIO/Validators/Base.hpp"

namespace ConfigIO
{
namespace Validators
{

struct ConsoleAppender: public Base
{
  virtual void validate(const AppOptions  &opts) const;

  virtual const char *typeToValidate(void) const;
}; // class ConsoleAppender

} // namespace Validators
} // namespace ConfigIO

#endif
