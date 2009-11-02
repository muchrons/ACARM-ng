/*
 * NodeConf.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONF_HPP_FILE
#define INCLUDE_LOGGER_NODECONF_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Appenders/Base.hpp"
#include "Logger/Formatter.hpp"
#include "Logger/Synchro.hpp"

namespace Logger
{

class NodeConf: private boost::noncopyable
{
public:
  explicit NodeConf(Appenders::BasePtr appender);

  void swap(NodeConf &other);

  Appenders::BasePtr getAppender() const
  {
    Lock lock(mutex_);
    return appender_;
  }
  Formatter getFormatter() const
  {
    Lock lock(mutex_);
    return formatter_;
  }
private:
  mutable Mutex      mutex_;
  Appenders::BasePtr appender_;
  Formatter          formatter_;    // note: formatter is held by value since
                                    //       at this moment it has just one
                                    //       possible instance and is not planned
                                    //       to expand like appenders' case.
};


typedef boost::shared_ptr<NodeConf> NodeConfPtr;

} // namespace Logger

#endif
