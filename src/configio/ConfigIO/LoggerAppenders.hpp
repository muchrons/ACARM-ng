/*
 * LoggerAppenders.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERAPPENDERS_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERAPPENDERS_HPP_FILE

/* public header */

#include <vector>
#include <string>

#include "ConfigIO/LoggerAppenderConfig.hpp"


namespace ConfigIO
{

/** \brief exception informing about non-existing appender
 */
struct ExceptionInvalidAppenderName: public Exception
{
  /** \brief create exception of invalid appender name
   *  \param where place where problem has been detected.
   *  \param name  name that does not exist
   */
  ExceptionInvalidAppenderName(const char        *where,
                               const std::string &name):
    Exception( std::string( ensureValidString(where) ) +
               ": appender name does not exist: " + name )
  {
  }
}; // struct ExceptionInvalidAppenderName

/** \brief exception informing about duplicated entries
 */
struct ExceptionInvalidDuplicatedAppenderName: public Exception
{
  /** \brief create exception of invalid appender name
   *  \param where place where problem has been detected.
   *  \param name  name that does not exist
   */
  ExceptionInvalidDuplicatedAppenderName(const char        *where,
                                         const std::string &name):
    Exception( std::string( ensureValidString(where) ) +
               ": duplicated appender name: " + name )
  {
  }
}; // struct ExceptionInvalidDuplicatedAppenderName


/** \brief collection of appenders configurations.
 */
class LoggerAppenders
{
public:
  /** \brief appenders' configuration collection.
   */
  typedef std::vector<LoggerAppenderConfig> Appenders;

  /** \brief const iterator declaration.
   */
  typedef Appenders::const_iterator const_iterator;

  /** \brief creates set of logger appenders' configuration.
   *  \param apps collection of appenders' configuration.
   *  \note if entries in apps will not be unique, c-tor will throw.
   */
  explicit LoggerAppenders(const Appenders &apps);

  /** \brief gets const iterator to collection's begin.
   *  \return beign const iterator.
   */
  const_iterator begin(void) const
  {
    return apps_.begin();
  }

  /** \brief gets const iterator to collection's end.
   *  \return end const iterator.
   */
  const_iterator end(void) const
  {
    return apps_.end();
  }

  /** \brief gets appender of a given name.
   *  \param name name of appender to be returned.
   *  \return appender with a given name.
   */
  const LoggerAppenderConfig &operator[](const std::string &name) const;

private:
  Appenders apps_;
}; // class LoggerAppenders

} // namespace ConfigIO

#endif
