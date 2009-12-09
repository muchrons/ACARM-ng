/*
 * NodeConf.cpp
 *
 */

#include "Logger/NodeConf.hpp"

namespace Logger
{

NodeConf::NodeConf(Appenders::BasePtr appender):
  appender_(appender)
{
}

void NodeConf::swap(NodeConf &other)
{
  Base::Threads::Lock lock(mutex_);
  appender_.swap(other.appender_);
  formatter_.swap(other.formatter_);
}

} // namespace Logger
