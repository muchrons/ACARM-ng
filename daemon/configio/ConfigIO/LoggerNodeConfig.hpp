/*
 * LoggerNodeConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERNODECONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERNODECONFIG_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/Exception.hpp"
#include "ConfigIO/OptionalString.hpp"


namespace ConfigIO
{

/** \brief class representing configuration of single node.
 */
class LoggerNodeConfig
{
public:
  /** \brief exception informing that no appender is assigned ot given node.
   */
  struct ExceptionNoValueAssigned: public Exception
  {
    /** \brief create exception of invalid appender name.
     *  \param where       place where problem has been detected.
     *  \param nodeName    name that does not exist.
     *  \param elementName name of element that has been not set.
     */
    ExceptionNoValueAssigned(const Location    &where,
                             const std::string &nodeName,
                             const std::string &elementName):
      Exception(where, elementName + " not assigned to node: " + nodeName)
    {
    }
  }; // struct ExceptionNoValueAssigned


  /** \brief creates configuration for given node.
   *  \param nodeName     name of a node.
   *  \param appenderName name of appender assigned to this node.
   *  \param threshold    threshold value for logging priority.
   */
  explicit LoggerNodeConfig(const std::string    &nodeName,
                            const OptionalString &appenderName=OptionalString(),
                            const OptionalString &threshold   =OptionalString() ):
    nodeName_(nodeName),
    appenderName_(appenderName),
    threshold_(threshold)
  {
  }

  /** \brief checks if appender is assigned to this node.
   *  \return true if appender is assigned, false otherwise.
   */
  bool hasAppender(void) const
  {
    return appenderName_.isSet();
  }
  /** \brief gets name of appender.
   *  \return name of assigned appender.
   */
  const std::string &getAppenderName(void) const
  {
    if( !hasAppender() )
      throw ExceptionNoValueAssigned(SYSTEM_SAVE_LOCATION, getNodeName(), "appender");
    return appenderName_.get();
  }
  /** \brief gets name of this node.
   *  \return name of node.
   */
  const std::string &getNodeName(void) const
  {
    return nodeName_;
  }
  /** \brief checks if threshold level has been set.
   *  \return ture if threshold is set, false otherwise.
   */
  bool hasThreshold(void) const
  {
    return threshold_.isSet();
  }
  /** \brief gets threshold value.
   *  \return priority threshold value.
   */
  const std::string &getThresholdValue(void) const
  {
    if( !hasThreshold() )
      throw ExceptionNoValueAssigned(SYSTEM_SAVE_LOCATION, getNodeName(), "threshold");
    return threshold_.get();
  }

private:
  std::string    nodeName_;
  OptionalString appenderName_;
  OptionalString threshold_;
}; // class LoggerNodeConfig

} // namespace ConfigIO

#endif
