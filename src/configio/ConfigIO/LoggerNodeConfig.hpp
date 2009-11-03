/*
 * LoggerNodeConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERNODECONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERNODECONFIG_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/Exception.hpp"
#include "ConfigIO/LoggerAppenders.hpp"


namespace ConfigIO
{

/** \brief exception informing that no appender is assigned ot given node.
 */
struct ExceptionNoAppenderAssigned: public Exception
{
  /** \brief create exception of invalid appender name
   *  \param where place where problem has been detected.
   *  \param name  name that does not exist
   */
  ExceptionNoAppenderAssigned(const char        *where,
                              const std::string &nodeName):
    Exception( std::string( ensureValidString(where) ) +
               ": appender not assigned to node: " + nodeName)
  {
  }
}; // struct ExceptionNoAppenderAssigned


/** \brief class representing configuration of single node.
 */
class LoggerNodeConfig
{
public:
  /** \brief creates configuration for given node.
   *  \param nodeName     name of a node.
   *  \param appenderName name of appender assigned to this node.
   */
  LoggerNodeConfig(const std::string &nodeName,
                   const std::string &appenderName):
    nodeName_(nodeName),
    appenderName_(appenderName),
    hasAppender_(true)
  {
  }

  /** \brief creates configuration for given node.
   *  \param nodeName     name of a node.
   */
  explicit LoggerNodeConfig(const std::string &nodeName):
    nodeName_(nodeName),
    hasAppender_(false)
  {
  }

  /** \brief checks if appender is assigned to this node.
   *  \return true if appender is assigned, false otherwise.
   */
  bool hasAppender(void) const
  {
    return hasAppender_;
  }

  /** \brief gets name of appender.
   *  \return name of assigned appender.
   */
  const std::string &getAppenderName(void) const
  {
    if( !hasAppender() )
      throw ExceptionNoAppenderAssigned("LoggerNodeConfig::getAppenderName()",
                                        getNodeName() );
    return appenderName_;
  }

  /** \brief gets name of this node.
   *  \return name of node.
   */
  const std::string &getNodeName(void) const
  {
    return nodeName_;
  }

private:
  std::string nodeName_;
  std::string appenderName_;
  bool        hasAppender_;
}; // class LoggerAppenderConfig

} // namespace ConfigIO

#endif
