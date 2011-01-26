/*
 * DynamicConfig.cpp
 *
 */
#include "Persistency/IO/Stubs/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

DynamicConfig::DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t, int /*handle*/):
  Persistency::IO::DynamicConfig(owner, t),
  callsWrite_(0),
  callsRead_(0),
  callsReadConst_(0)
{
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
{
  ++callsWrite_;
  owner_[ getOwner().get() ][ key.get() ]=value.get();
}

DynamicConfig::ValueNULL DynamicConfig::readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
{
  ++callsRead_;
  DataMap &data=owner_[ getOwner().get() ];
  if( data.find( key.get() )==data.end() )
    return ValueNULL();
  return ValueNULL( data[ key.get() ] );
}

DynamicConfig::Value DynamicConfig::readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  ++callsReadConst_;
  return "i'm const";
}

void DynamicConfig::removeImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
{
  DataMap &data=owner_[ getOwner().get() ];
  data.erase( key.get() );
}

void DynamicConfig::iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
{
  DataMap &data=owner_[ getOwner().get() ];
  for(DataMap::const_iterator it=data.begin(); it!=data.end(); ++it)
    if( cb.process(it->first, it->second)==false )
      return;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
