/*
 * LoggerAppenderConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERAPPENDERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERAPPENDERCONFIG_HPP_FILE

/* public header */

#include <map>
#include <string>

#include "ConfigIO/Exception.hpp"


namespace ConfigIO
{

/** \brief exception informing about non-existing option
 */
struct ExceptionInvalidOptionName: public Exception
{
  /** \brief create exception of invalid option
   *  \param where  place where problem has been detected.
   *  \param option name of an option that was not found.
   */
  ExceptionInvalidOptionName(const char        *where,
                             const std::string &option):
    Exception( std::string( ensureValidString(where) ) +
               ": invalid option requested: " + option )
  {
  }
}; // struct ExceptionInvalidPortNumber


class LoggerAppenderConfig
{
public:
  typedef std::map<std::string, std::string> Options;

  LoggerAppenderConfig(const std::string &type,
                       const std::string &name,
                       const Options     &opts);

  const std::string &getType(void) const
  {
    return type_;
  }

  const std::string &getName(void) const
  {
    return name_;
  }

  const std::string &operator[](const std::string &name) const;

private:
  std::string type_;
  std::string name_;
  Options     opts_;
}; // class LoggerAppenderConfig

} // namespace ConfigIO

#endif
