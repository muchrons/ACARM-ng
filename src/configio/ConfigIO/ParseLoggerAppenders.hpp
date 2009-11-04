/*
 * ParseLoggerAppenders.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE

/* pulic header */

#include "XML/Node.hpp"
#include "ConfigIO/LoggerAppenders.hpp"
#include "ConfigIO/Exception.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace ConfigIO
{

/** \brief exception informing about no appenders registered.
 */
struct ExceptionNoAppendersDefined: public Exception
{
  /** \brief create exception of no appenders defined
   *  \param where  place where problem has been detected.
   */
  ExceptionNoAppendersDefined(const char *where):
    Exception( std::string( ensureValidString(where) ) +
               ": at least one appender must be defined" )
  {
  }
}; // struct ExceptionNoAppendersDefined



class ParseLoggerAppenders
{
public:
  ParseLoggerAppenders(const XML::Node &node);

  const LoggerAppenders &getAppenders(void) const
  {
    return appenders_;
  }

private:
  LoggerAppenders parse(const XML::Node &node) const;

  LoggerAppenders appenders_;
}; // class ParseLoggerAppenders

} // namespace ConfigIO

#endif
