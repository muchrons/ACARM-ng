/*
 * NodeConf.cpp
 *
 */
#include "Base/Threads/WriteLock.hpp"
#include "Logger/NodeConf.hpp"

namespace Logger
{

NodeConf::NodeConf(Appenders::BasePtr appender, Priority threshold):
  appender_(appender),
  threshold_(threshold)
{
}

void NodeConf::swap(NodeConf &other)
{
  Base::Threads::WriteLock lock(mutex_);
  appender_.swap(other.appender_);
  formatter_.swap(other.formatter_);
  // threshold is only a simple value
  {
    const Priority tmp=threshold_;
    threshold_      =other.threshold_;
    other.threshold_=tmp;
  }
}

} // namespace Logger
