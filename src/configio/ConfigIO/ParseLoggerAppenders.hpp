/*
 * ParseLoggerAppenders.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSELOGGERAPPENDERS_HPP_FILE

/* pulic header */

#include "XML/Node.hpp"
#include "ConfigIO/LoggerAppenders.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace ConfigIO
{

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
