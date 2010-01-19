/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_TESTBASE_T_HPP
#define INCLUDE_TESTBASE_T_HPP

#include <tut/tut.hpp>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

namespace
{

struct TestBase
{
  TestBase(void):
    defaultFile_("acarm_ng_config.xml")
  {
    unlink(defaultFile_);
    std::stringstream ss;
    ss<<"cp 'testdata/sample_config.xml' '"<<defaultFile_<<"'";
    tut::ensure_equals("copying file as default config failed",
                       system( ss.str().c_str() ), 0);
  }
  ~TestBase(void)
  {
    unlink(defaultFile_);
  }

  const char *defaultFile_;
}; // struct TestBase
} // unnmed namespace

#endif
