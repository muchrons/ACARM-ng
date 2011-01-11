/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_TESTBASE_T_HPP_FILE
#define INCLUDE_PLUGINS_TESTBASE_T_HPP_FILE

#include <boost/filesystem.hpp>

#include "System/Plugins/Builder.hpp"

namespace
{

struct TestBase
{
  TestBase(void):
    dynObj_( builder_.open("testdata/sharedobj.plugin") )
  {
  }

  boost::filesystem::path compile(const boost::filesystem::path &src)
  {
    tut::ensure_equals("compilation of helper module failed",
                       system("gcc -g3 -Wall -shared -o sharedobj.so -fPIC testdata/sharedobj.c"),
                       0);
  }

  System::Plugins::Builder       builder_;
  System::Plugins::DynamicObject dynObj_;
};

} // unnamed namespace

#endif
