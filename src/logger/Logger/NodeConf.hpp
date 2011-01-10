/*
 * NodeConf.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONF_HPP_FILE
#define INCLUDE_LOGGER_NODECONF_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Base/Threads/ReadWriteMutex.hpp"
#include "Base/Threads/ReadLock.hpp"
#include "Logger/Appenders/Base.hpp"
#include "Logger/Formatter.hpp"
#include "Logger/Priority.hpp"
#include "Logger/NodeConfPtr.hpp"

namespace Logger
{

/** \brief class representing configuration of a given node.
 */
class NodeConf: private boost::noncopyable
{
public:
  /** \brief create configariotn with a given appender and default formatter.
   *  \param appender  appender to be in node configuration.
   *  \param formatter formatter to be used for this config.
   *  \param threshold minimum level that has to be reported to be logged.
   */
  NodeConf(Appenders::BasePtr appender, FormatterPtr formatter, Priority threshold);
  /** \brief swaps contents of this node conf and one given as parameter.
   *  \param other node conf to swap content with.
   */
  void swap(NodeConf &other);
  /** \brief gives access to appender held inside.
   *  \return appender for this configuration.
   */
  Appenders::BasePtr getAppender(void) const
  {
    Base::Threads::ReadLock lock(mutex_);
    return appender_;
  }
  /** \brief gives access to formatter.
   *  \return formatter.
   */
  FormatterPtr getFormatter(void) const
  {
    Base::Threads::ReadLock lock(mutex_);
    return formatter_;
  }
  /** \brief gets threshold level for logging.
   *  \return threshold level.
   */
  Priority getThreshold(void) const
  {
    Base::Threads::ReadLock lock(mutex_);
    return threshold_;
  }

private:
  mutable Base::Threads::ReadWriteMutex mutex_;
  Appenders::BasePtr                    appender_;
  Priority                              threshold_;
  FormatterPtr                          formatter_;
}; // class NodeConf

} // namespace Logger

#endif
