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
#include "Persistency/TestBase.t.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass: private Persistency::TestBase
{
  TestClass(void)
  {
    const char *defaultFile="test_config.xml";
    unlink(defaultFile);
    stringstream ss;
    ss<<"cp 'testdata/sample_config.xml' '"<<defaultFile<<"'";
    tut::ensure_equals("copying file as default config failed",
                       system( ss.str().c_str() ), 0);

  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/BackendFactory");
} // unnamed namespace


namespace tut
{

// test if we're able to create factory defined in config file
template<>
template<>
void testObj::test<1>(void)
{
  ensure("NULL pointer created", create().get()!=NULL );
}

} // namespace tut
