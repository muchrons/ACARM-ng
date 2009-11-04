/*
 * LoggerAppenderConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERAPPENDERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERAPPENDERCONFIG_HPP_FILE

/* public header */

#include <map>
#include <vector>
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
}; // struct ExceptionInvalidOptionName


/** \brief class representing configuration of single appender.
 */
class LoggerAppenderConfig
{
public:
  /** \brief values vector for given option.
   */
  typedef std::vector<std::string>               ParameterValues;

  /** \brief parameters map (name => values).
   */
  typedef std::map<std::string, ParameterValues> Options;

  /** \brief creates configuration for given appender.
   *  \param type name of appender's type.
   *  \param name name of appender instance.
   *  \param opts options for this appender.
   */
  LoggerAppenderConfig(const std::string &type,
                       const std::string &name,
                       const Options     &opts);

  /** \brief gets type of appender.
   *  \return type name of this appender.
   */
  const std::string &getType(void) const
  {
    return type_;
  }

  /** \brief gets name of this appender instance.
   *  \return name of appender.
   */
  const std::string &getName(void) const
  {
    return name_;
  }

  /** \brief returns option of a given type or throws if it does not exist.
   *  \param name name of parameter to get value of.
   *  \return parameter's value.
   */
  const ParameterValues &operator[](const std::string &name) const;

private:
  std::string type_;
  std::string name_;
  Options     opts_;
}; // class LoggerAppenderConfig

} // namespace ConfigIO

#endif
