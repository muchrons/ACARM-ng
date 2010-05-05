/*
 * TestBase.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_TESTBASE_HPP
#define INCLUDE_TESTHELPERS_TESTBASE_HPP

/* public header */

#include <stdlib.h>

namespace TestHelpers
{

/** \brief base class for tests - creates default config file.
 */
class TestBase
{
public:
  TestBase(const char *xml=NULL);
  ~TestBase(void);

  void defaultConfig(void);

  void readConfigFile(const char *path);

private:
  const char *defaultFile_;
}; // struct TestBase
} // namespace TestHelpers

#endif
