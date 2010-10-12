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

} // namespace Persistency
} // namespace TestHelpers
