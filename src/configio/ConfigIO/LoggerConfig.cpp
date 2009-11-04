/*
 * LoggerConfig.cpp
 *
 */
#include "ConfigIO/LoggerConfig.hpp"

namespace ConfigIO
{

LoggerConfig::LoggerConfig(const LoggerNodeConfig &defaultNode,
                           const LoggerNodes      &nodes,
                           const LoggerAppenders  &appenders):
  defaultNode_(defaultNode),
  nodes_(nodes),
  appenders_(appenders)
{
  // root node should not have name
  if( defaultNode_.getNodeName()!="" )
    throw ExceptionBadOrMissingDefaultNodeSetting(
                            "LoggerConfig::LoggerConfig()",
                            "node name", defaultNode_.getNodeName() );

  // default not must have appender set
  if( !defaultNode_.hasAppender() )
    throw ExceptionBadOrMissingDefaultNodeSetting(
                            "LoggerConfig::LoggerConfig()",
                            "appender", "<none>");

  // check if appender name is valid (if not following call will throw.
  appenders_[defaultNode_.getAppenderName()];

  // for each node check if corresponging appender exists
  for(LoggerNodes::const_iterator it=nodes_.begin(); it!=nodes_.end(); ++it)
    if( it->hasAppender() )                 // check only if appenders exist
      appenders_[it->getAppenderName()];    // will throw on error
}

} // namespace ConfigIO
