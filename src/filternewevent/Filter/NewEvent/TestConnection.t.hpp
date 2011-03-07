/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TESTCONNECTION_T_HPP_FILE

#include <cassert>

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "Filter/NewEvent/TestDynamicConfigStub.t.hpp"

namespace Filter
{
namespace NewEvent
{

class TestConnection: public TestHelpers::Persistency::ConnectionUserStubBase
{
public:
  TestDynamicConfigStub::DataPtr data_; // storage of lastly created DC-Stub

private:
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    TestDynamicConfigStub                   *tmp=new TestDynamicConfigStub(owner, t);
    ::Persistency::IO::DynamicConfigAutoPtr  ptr(tmp);
    assert(tmp!=NULL);
    data_=tmp->getDataPtr();
    return ptr;
  }
}; // class TestConnection


} // namespace Filter
} // namespace NewEvent

#endif
