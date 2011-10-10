/*
 * TestStubs.cpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_TESTSTUBS_HPP
#define INCLUDE_TESTHELPERS_PERSISTENCY_TESTSTUBS_HPP

/* public header */

#include "TestHelpers/TestBase.hpp"

namespace TestHelpers
{
namespace Persistency
{

/** \brief base class for tests - creates config file for persistency stubs.
 */
class TestStubs: public TestBase
{
public:
  TestStubs(void);
}; // struct TestStubs

} // namespace Persistency
} // namespace TestHelpers

#endif
