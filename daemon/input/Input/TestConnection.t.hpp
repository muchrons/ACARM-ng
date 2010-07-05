/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_INPUT_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_INPUT_TESTCONNECTION_T_HPP_FILE

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "Input/TestDynamicConfigStub.t.hpp"

namespace Input
{

class TestConnection: public TestHelpers::Persistency::ConnectionUserStubBase
{
private:
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    return ::Persistency::IO::DynamicConfigAutoPtr( new TestDynamicConfigStub(owner, t) );
  }
}; // class TestConnection


inline Persistency::IO::ConnectionPtrNN createUserStub(void)
{
  return Persistency::IO::ConnectionPtrNN(new TestConnection);
} // createUserStub()

} // namespace Input

#endif
