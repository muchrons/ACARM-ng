/*
 * Console.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE

#include <iosfwd>

#include "Logger/Appenders/Stream.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief console logger - messages are displayed on the screen.
 */
class Console: public Stream<Console>
{
public:
  /** \brief creates console appender.
   */
  Console(void);

  /** \brief static method for getting type name.
   *  \return compile-time pointer to type name.
   */
  static const char *getThisTypeName(void)
  {
    return "console";
  }

private:
  virtual void reinitImpl(void);
}; // class Console

} // namespace Appenders
} // namespace Logger

#endif
