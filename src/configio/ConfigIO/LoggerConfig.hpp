/*
 * LoggerConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERCONFIG_HPP_FILE

/* public header */

#include "ConfigIO/LoggerAppenders.hpp"
#include "ConfigIO/LoggerNodes.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace ConfigIO
{

/** \brief exception informing about bad or missing option for default node.
 */
struct ExceptionBadOrMissingDefaultNodeSetting: public Exception
{
  /** \brief create exception for bad or missing default node option.
   *  \param where place where problem has been detected.
   *  \param name  name of bad or missing option.
   *  \param value if value is bad, show it.
   */
  ExceptionBadOrMissingDefaultNodeSetting(const char        *where,
                                          const char        *name,
                                          const std::string &value):
    Exception( std::string( ensureValidString(where) ) +
        ": bad or missing oprion: " + ensureValidString(name) +
        " (" + value + ")")
  {
  }
}; // struct ExceptionBadOrMissingDefaultNodeSetting


/** \brief class representing logger's configuration.
 */
class LoggerConfig
{
public:
  LoggerConfig(const LoggerNodeConfig &defaultNode,
               const LoggerNodes      &nodes,
               const LoggerAppenders  &appenders);

  const LoggerNodeConfig getDefaultNodeConfig(void) const
  {
    return defaultNode_;
  }

  const LoggerNodes &getNodes(void) const
  {
    return nodes_;
  }

  const LoggerAppenders &getAppenders(void) const
  {
    return appenders_;
  }

private:
  LoggerNodeConfig defaultNode_;
  LoggerNodes      nodes_;
  LoggerAppenders  appenders_;
}; // class LoggerConfig

} // namespace ConfigIO

#endif
