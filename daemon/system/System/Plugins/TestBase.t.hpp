/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_TESTBASE_T_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_TESTBASE_T_HPP_FILE

#include <tut/tut.hpp>
#include <dlfcn.h>

#include "System/Plugins/Handle.hpp"

namespace
{

struct TestBase
{
  TestBase(void):
    h_( new System::Plugins::Handle( openShared() ) )
  {
  }

  void *openShared(void) const
  {
    dlerror();
    void *h=dlopen("testdata/sharedobj.so", RTLD_LOCAL|RTLD_LAZY);
    if(h==NULL)
      tut::fail( dlerror() );
    return h;
  }

  System::Plugins::HandlePtrNN h_;
};

} // unnamed namespace

#endif
