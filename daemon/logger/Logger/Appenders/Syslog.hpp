/*
 * Syslog.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_SYSLOG_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_SYSLOG_HPP_FILE

#include "Logger/Appenders/Base.hpp"

namespace Logger
{
namespace Appenders
{

/** \brief sends logged messages to syslog.
 */
class Syslog: public Base
{
public:
  /** \brief static method for getting type name.
   *  \return compile-time pointer to type name.
   */
  static const char *getThisTypeName(void)
  {
    return "syslog";
  }

private:
  virtual void appendImpl(const std::string &str);
  virtual const char *getTypeNameImpl(void) const;
}; // class Syslog

} // namespace Appenders
} // namespace Logger

#endif
