/*
 * IODynamicConfigMemory.cpp
 *
 */
#include <tut/tut.hpp>

#include "TestHelpers/Persistency/IODynamicConfigMemory.hpp"

namespace TestHelpers
{
namespace Persistency
{

IODynamicConfigMemory::IODynamicConfigMemory(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                             ::Persistency::IO::Transaction                &t,
                                             Memory                                        &mem):
  ::Persistency::IO::DynamicConfig(owner, t),
  mem_(mem)
{
}

void IODynamicConfigMemory::writeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
{
  mem_[ key.get() ]=value.get();
}

IODynamicConfigMemory::ValueNULL IODynamicConfigMemory::readImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
{
  if( mem_.find( key.get() )==mem_.end() )
    return ValueNULL();
  return ValueNULL( mem_[ key.get() ] );
}

IODynamicConfigMemory::Value IODynamicConfigMemory::readConstImpl(::Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  tut::fail("readConst() should NOT be called at all");
  return Value("???");
}

void IODynamicConfigMemory::removeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
{
  mem_.erase( key.get() );
}

void IODynamicConfigMemory::iterateImpl(::Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
{
  for(Memory::const_iterator it=mem_.begin(); it!=mem_.end(); ++it)
    if( cb.process(it->first, it->second)==false )
      return;
}

} // namespace Persistency
} // namespace TestHelpers
