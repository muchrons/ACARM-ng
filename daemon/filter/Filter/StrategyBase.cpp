/*
 * StrategyBase.cpp
 *
 */
#include <boost/thread.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/StrategyBase.hpp"

using namespace std;


namespace Filter
{

StrategyBase::~StrategyBase(void)
{
  LOGMSG_INFO(log_, "deallocating filter");
}

namespace
{
inline Logger::NodeName makeNodeName(const string &type, const string &name)
{
  const string &out="filter." + Logger::NodeName::removeInvalidChars(type + "." + name);
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

StrategyBase::StrategyBase(const std::string &type, const std::string &name):
  log_( makeNodeName(type, name) ),
  conn_( Persistency::IO::create() ),
  type_(type)
{
  LOGMSG_INFO(log_, "creating filter");
}

void StrategyBase::interruptionPoint(void) const
{
  boost::this_thread::interruption_point();
}

} // namespace Filter
