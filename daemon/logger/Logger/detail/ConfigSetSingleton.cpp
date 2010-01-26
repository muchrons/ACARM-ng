/*
 * ConfigSetSingleton.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Logger/detail/ConfigSetSingleton.hpp"
#include "Logger/nodeConfReader.hpp"

using namespace std;
using namespace ConfigIO;

namespace Logger
{
namespace detail
{

NodeConfPtr ConfigSetImpl::getNodeConfiguration(const NodeName &nn)
{
  Base::Threads::Lock lock(mutex_);

  ConfigMap::const_iterator it=cfgMap_.find( nn.get() );// try getting entry

  // if entry does not exist, it has to be created.
  if( it==cfgMap_.end() )
  {
    NodeConfPtr nc=nodeConfReader(nn, appMap_);         // process configuration
    assert( nc.get()!=NULL );
    cfgMap_.insert( make_pair(nn.get(), nc) );          // add it to map
    it=cfgMap_.find( nn.get() );                        // get newly added entry
  } // if(entry does not exist)

  assert( it!=cfgMap_.end() );
  assert( nn.get()==it->first );

  return it->second;
}

ConfigSetImpl::ConfigSetImpl(void):
  appMap_( ConfigIO::Singleton::get()->loggerConfig().getAppenders() )
{
}

} // namespace detail
} // namespace Logger
