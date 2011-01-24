/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_TESTBASE_T_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_TESTBASE_T_HPP_FILE

#include <tut/tut.hpp>
#include <cstdlib>
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
    // compile module
    tut::ensure_equals("compilation of helper module failed",
                       system("gcc -g3 -Wall -shared -o sharedobj.so -fPIC testdata/sharedobj.c"),
                       0);

    // open it
    dlerror();
    void *h=dlopen("./sharedobj.so", RTLD_LOCAL|RTLD_LAZY);
    if(h==NULL)
      tut::fail( dlerror() );
    return h;
  }

  System::Plugins::HandlePtrNN h_;
};

} // unnamed namespace

#endif
