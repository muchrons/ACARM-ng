/*
 * Console.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE

#include "Logger/Appenders/Base.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief console logger - messages are displayed on the screen.
 */
class Console: public Base
{
private:
  virtual void appendImpl(const std::string &str);
  virtual const char *getTypeNameImpl(void) const;
}; // class Console

} // namespace Appenders
} // namespace Logger

#endif
