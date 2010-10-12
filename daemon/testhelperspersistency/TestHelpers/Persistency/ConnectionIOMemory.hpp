/*
 * ConnectionIOMemory.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONIOMEMORY_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONIOMEMORY_HPP_FILE

/* public header */

#include <map>
#include <string>
#include <cassert>

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "TestHelpers/Persistency/IODynamicConfigMemory.hpp"

namespace TestHelpers
{
namespace Persistency
{

struct ConnectionIOMemory: public ConnectionUserStubBase
{
  typedef std::map<std::string, IODynamicConfigMemory::Memory> OwnerToMemory;

  // initialize internal counters
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    assert( owner.get()!=NULL );
    return ::Persistency::IO::DynamicConfigAutoPtr( new IODynamicConfigMemory(owner, t, otm_[ owner.get() ]) );
  }

  OwnerToMemory otm_;
}; // struct ConnectionIOMemory

} // namespace Persistency
} // namespace TestHelpers

#endif
