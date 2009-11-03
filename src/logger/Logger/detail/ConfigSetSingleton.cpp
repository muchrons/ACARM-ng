/*
 * ConfigSetSingleton.cpp
 *
 */
#include <cassert>

#include "Logger/detail/ConfigSetSingleton.hpp"
#include "Logger/Appenders/Console.hpp"             // TODO: this is temporary

using namespace std;


namespace Logger
{
namespace detail
{

ConfigSetImpl::ConfigSetImpl(void)
{
  //
  // TODO: hardcoded console output is temporaray. it is to be replaced
  //       with file configuration that can be parsed and changed in runtime.
  //
  Appenders::BasePtr console(new Appenders::Console);
  appMap_.insert( make_pair(console->getTypeName(), console) );
}


NodeConfPtr ConfigSetImpl::getNodeConfiguration(const NodeName &nn)
{
  ConfigMap::const_iterator it=cfgMap_.find( nn.get() ); // try getting entry
  // if entry does not exist, it has to be created.
  if( it==cfgMap_.end() )
  {
    assert( appMap_.size()==1 && "appenders map has been updated, but code not");
    Appenders::BasePtr app=appMap_.begin()->second;
    assert(app.get()!=NULL);
    NodeConfPtr        nc( new NodeConf(app) ); // create entry
    cfgMap_.insert( make_pair(nn.get(), nc) );  // add it to map
    it=cfgMap_.find( nn.get() );                // get newly added entry
  } // if(entry does not exist)

  assert( it!=cfgMap_.end() );
  assert( nn.get()==it->first );

  return it->second;
}

} // namespace detail
} // namespace Logger
