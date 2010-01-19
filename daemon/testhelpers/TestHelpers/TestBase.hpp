/*
 * TestBase.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_TESTBASE_HPP
#define INCLUDE_TESTHELPERS_TESTBASE_HPP

/* public header */

namespace TestHelpers
{

/** \brief base class for tests - creates default config file.
 */
class TestBase
{
public:
  TestBase(bool useStub=false);
  ~TestBase(void);

private:
  const char *defaultFile_;
}; // struct TestBase
} // namespace TestHelpers

#endif
