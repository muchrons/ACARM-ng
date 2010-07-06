/*
 * TestBase.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_INPUT_TESTBASE_HPP_FILE
#define INCLUDE_TESTHELPERS_INPUT_TESTBASE_HPP_FILE

/* public header */

#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Input/BackendFacadeStub.hpp"

namespace TestHelpers
{
namespace Input
{

struct TestBase: public Persistency::TestStubs
{
  TestBase(void);

  ::Persistency::IO::ConnectionPtrNN conn_;
  ::Persistency::IO::Transaction     t_;
  BackendFacadeStub                  bf_;
};

} // namespace Input
} // namespace TestHelpers

#endif
