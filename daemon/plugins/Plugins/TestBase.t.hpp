/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_TESTBASE_T_HPP_FILE
#define INCLUDE_PLUGINS_TESTBASE_T_HPP_FILE

#include "System/Plugins/Builder.hpp"

namespace
{

struct TestBase
{
  TestBase(void):
    dynObj_( builder_.open("testdata/sharedobj.so") )
  {
  }

  System::Plugins::Builder       builder_;
  System::Plugins::DynamicObject dynObj_;
};

} // unnamed namespace

#endif
