/*
 * LoggerConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERCONFIG_HPP_FILE

/* public header */

#include "ConfigIO/LoggerAppenders.hpp"
#include "ConfigIO/LoggerNodes.hpp"

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
  /** \brief create logger's configuration.
   *  \param defaultNode configuration for default node.
   *  \param nodes       nodes' configuration.
   *  \param appenders   appenders/ configuration.
   *  \note if nodes reflect to non-existing appenders, c-tor will throw.
   */
  LoggerConfig(const LoggerNodeConfig &defaultNode,
               const LoggerNodes      &nodes,
               const LoggerAppenders  &appenders);

  /** \brief gets configuration of default node.
   *  \return default node's configuration.
   */
  const LoggerNodeConfig getDefaultNodeConfig(void) const
  {
    return defaultNode_;
  }

  /** \brief gets nodes' configuration.
   *  \return configuration for nodes.
   */
  const LoggerNodes &getNodes(void) const
  {
    return nodes_;
  }

  /** \brief gets appenders' configuration.
   *  \return configuration for appenders.
   */
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
