/*
 * ParseLoggerAppenders.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/LoggerAppenders.hpp"
#include "ConfigIO/Exception.hpp"

namespace ConfigIO
{

/** \brief exception informing about no appenders registered.
 */
struct ExceptionNoAppendersDefined: public Exception
{
  /** \brief create exception of no appenders defined
   *  \param where  place where problem has been detected.
   */
  explicit ExceptionNoAppendersDefined(const Location &where):
    Exception(where, "at least one appender must be defined")
  {
  }
}; // struct ExceptionNoAppendersDefined


/** \brief parser of XML subtree corresponding to logger
 *         appenders' configuration.
 */
class ParseLoggerAppenders
{
public:
  /** \brief parse subtree and save results in class.
   *  \param node root node to start parsing from.
   *  \note this is internal implementation and therefore assumes that
   *        given subtree is valid in logger's configuration XML tree.
   */
  explicit ParseLoggerAppenders(const XML::Node &node);

  /** \brief gets appenders' configuration that has been parsed.
   *  \return appenders' configuration.
   */
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
