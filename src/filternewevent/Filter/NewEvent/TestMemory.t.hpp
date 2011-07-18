/*
 * TestMemory.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TESTMEMORY_T_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TESTMEMORY_T_HPP_FILE

#include <cassert>

#include "TestHelpers/Persistency/IODynamicConfigMemory.hpp"
#include "Filter/NewEvent/Exception.hpp"

namespace Filter
{
namespace NewEvent
{

// TODO: it is good this functionality is separated, though it has to be placed in unnaed namespace
//       otherwise linkage error will appear when included from multiple source files.

struct TestMemory: public TestHelpers::Persistency::IODynamicConfigMemory
{
  TestMemory(const ::Persistency::IO::DynamicConfig::Owner &owner,
             ::Persistency::IO::Transaction                &t,
             Memory                                        &mem):
    TestHelpers::Persistency::IODynamicConfigMemory(owner, t, mem)
  {
  }

  virtual void removeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    mem_.erase( key.get() );
    throw Exception(SYSTEM_SAVE_LOCATION, "test exception");
  }
}; // struct TestMemory

} // namespace Filter
} // namespace NewEvent

#endif
