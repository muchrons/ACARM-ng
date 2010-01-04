/*
 * Interface.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/Interface.hpp"

using namespace std;


namespace Filter
{

Interface::~Interface(void)
{
  LOGMSG_INFO(log_, "deallocating filter");
}

void Interface::process(Node n, ChangedNodes &changed)
{
  LOGMSG_DEBUG_S(log_)<<"processing node at address 0x"
                      <<static_cast<void*>( n.get() );
  BackendProxy bp( conn_, getFilterName() );
  assert( changed.size()==0 && "non-empty output collection received");
  processImpl(n, changed, ntq_, bp);
}


namespace
{
inline Logger::NodeName makeNodeName(const string &name)
{
  const string &out="filter.interface." + name;
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

Interface::Interface(const std::string &name):
  log_( makeNodeName(name) ),
  name_(name),
  conn_( Persistency::IO::create() )
{
  LOGMSG_INFO(log_, "creating filter");
}

} // namespace Filter
