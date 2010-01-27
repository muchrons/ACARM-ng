/*
 * NodeConf.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONF_HPP_FILE
#define INCLUDE_LOGGER_NODECONF_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
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
   *  \param threshold minimum level that has to be reported to be logged.
   */
  NodeConf(Appenders::BasePtr appender, Priority threshold);
  /** \brief swaps contents of this node conf and one given as parameter.
   *  \param other node conf to swap content with.
   */
  void swap(NodeConf &other);
  /** \brief gives access to appender held inside.
   *  \return appender for this configuration.
   */
  Appenders::BasePtr getAppender(void) const
  {
    Base::Threads::Lock lock(mutex_);
    return appender_;
  }
  /** \brief gives access to formatter.
   *  \return formatter.
   */
  Formatter getFormatter(void) const
  {
    Base::Threads::Lock lock(mutex_);
    return formatter_;
  }
  /** \brief gets threshold level for logging.
   *  \return threshold level.
   */
  Priority getThreshold(void) const
  {
    return threshold_;
  }

private:
  mutable Base::Threads::Mutex mutex_;
  Appenders::BasePtr           appender_;
  Priority                     threshold_;
  Formatter                    formatter_;  // note: formatter is held by value since
                                            //       at this moment it has just one
                                            //       possible instance and is not planned
                                            //       to expand like appenders' case.
}; // class NodeConf

} // namespace Logger

#endif
