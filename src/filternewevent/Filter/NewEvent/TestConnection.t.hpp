/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE

#include <cassert>

#include "TestHelpers/Persistency/ConnectionIOMemory.hpp"
#include "Filter/NewEvent/TestMemory.t.hpp"

namespace Filter
{
namespace NewEvent
{

// TODO: it is good this functionality is separated, though it has to be placed in unnaed namespace
//       otherwise linkage error will appear when included from multiple source files.

struct TestConnection: public TestHelpers::Persistency::ConnectionIOMemory
{
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    assert( owner.get()!=NULL );
    return ::Persistency::IO::DynamicConfigAutoPtr( new TestMemory(owner, t, otm_[ owner.get() ]) );
  }
}; // struct TestConnection

} // namespace Filter
} // namespace NewEvent

#endif
