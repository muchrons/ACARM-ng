/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE

#include <cassert>

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "TestHelpers/Persistency/IODynamicConfigMemory.hpp"

namespace Filter
{
namespace NewEvent
{

// TODO: TestHelpers::Persistency::IODynamicConfigMemory should be used instead.
class TestConnection: public TestHelpers::Persistency::ConnectionUserStubBase
{
public:
  TestHelpers::Persistency::IODynamicConfigMemory::Memory data_; // storage of lastly created DC-Stub

private:
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    TestHelpers::Persistency::IODynamicConfigMemory *tmp=new TestHelpers::Persistency::IODynamicConfigMemory(owner, t, data_);
    ::Persistency::IO::DynamicConfigAutoPtr  ptr(tmp);
    assert(tmp!=NULL);
    //data_=tmp->mem_;
    return ptr;
  }
}; // class TestConnection


} // namespace Filter
} // namespace NewEvent

#endif
