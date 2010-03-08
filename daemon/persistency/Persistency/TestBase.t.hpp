/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TESTBASE_T_HPP_FILE
#define INCLUDE_PERSISTENCY_TESTBASE_T_HPP_FILE

/* public header */

#include "TestHelpers/TestBase.hpp"

namespace Persistency
{

/** \brief base class for tests - creates default config file.
 */
class TestBase: public TestHelpers::TestBase
{
public:
  TestBase(void);
}; // struct TestBase

} // namespace Persistency

#endif
