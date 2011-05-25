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
using namespace Core::Types::Proc;


namespace Filter
{

StrategyBase::~StrategyBase(void)
{
  LOGMSG_INFO(log_, "deallocating filter");
}

namespace
{
inline Logger::NodeName makeNodeName(const TypeName &type, const InstanceName &name)
{
  const string &out="filter." + Logger::NodeName::removeInvalidChars(type.str() + "." + name.str());
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

StrategyBase::StrategyBase(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name):
  log_( makeNodeName(type, name) ),
  conn_( Persistency::IO::create() ),
  type_(type),
  name_(name)
{
  LOGMSG_INFO(log_, "creating filter");
}

void StrategyBase::interruptionPoint(void) const
{
  boost::this_thread::interruption_point();
}

} // namespace Filter
