/*
 * NodeConf.cpp
 *
 */
#include "Base/Threads/WriteLock.hpp"
#include "Logger/NodeConf.hpp"

namespace Logger
{

NodeConf::NodeConf(Appenders::BasePtr appender, FormatterPtr formatter, Priority threshold):
  appender_(appender),
  threshold_(threshold),
  formatter_(formatter)
{
}

void NodeConf::swap(NodeConf &other)
{
  // TODO: other's lock must be locked for writing here as well
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
