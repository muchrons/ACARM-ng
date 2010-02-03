/*
 * Strategy.cpp
 *
 */
#include <boost/thread.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/Strategy.hpp"

using namespace std;


namespace Filter
{

Strategy::~Strategy(void)
{
  LOGMSG_INFO(log_, "deallocating filter");
}

void Strategy::process(Node n, ChangedNodes &changed)
{
  LOGMSG_DEBUG_S(log_)<<"processing node at address 0x"
                      <<static_cast<void*>( n.get() );
  BackendProxy bp( conn_, changed, getFilterName() );
  assert( changed.size()==0 && "non-empty output collection received");
  processImpl(n, ntq_, bp);
}


namespace
{
inline Logger::NodeName makeNodeName(const string &name)
{
  const string &out="filter." + name;
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

Strategy::Strategy(const std::string &name):
  log_( makeNodeName(name) ),
  name_(name),
  conn_( Persistency::IO::create() )
{
  LOGMSG_INFO(log_, "creating filter");
}

void Strategy::interruptionPoint(void)
{
  boost::this_thread::interruption_point();
}

} // namespace Filter
