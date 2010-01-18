/*
 * BackendFactory.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

#include "Persistency/IO/BackendFactory.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
  TestClass(void)
  {
    const char *defaultFile="acarm_ng_config.xml";
    unlink(defaultFile);
    stringstream ss;
    ss<<"cp 'testdata/sample_config.xml' '"<<defaultFile<<"'";
    tut::ensure_equals("copying file as default config failed",
                       system( ss.str().c_str() ), 0);

  }
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/BackendFactory");
} // unnamed namespace


namespace tut
{

// creating of instance should not be possible, since at the moment no
// backend is registered.
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    create();
    fail("create() didn't throw when no factory is registered");
  }
  catch(const Commons::Factory::ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}

} // namespace tut
