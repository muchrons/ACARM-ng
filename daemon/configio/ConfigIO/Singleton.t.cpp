/*
 * Singleton.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

#include "ConfigIO/Singleton.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
const char *defaultFile="acarm_ng_config.xml";

struct TestClass
{
  TestClass(void)
  {
    unlink(defaultFile);

    stringstream ss;
    ss<<"cp 'testdata/sample_config.xml' '"<<defaultFile<<"'";
    tut::ensure_equals("copying file as default config failed",
                       system( ss.str().c_str() ), 0);
  }

  ~TestClass(void)
  {
    unlink(defaultFile);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Singleton");
} // unnamed namespace


namespace tut
{

// test getting singleton's implementation
template<>
template<>
void testObj::test<1>(void)
{
  ensure("NULL pointer received", Singleton::get()!=NULL );
}

// test getting logger's config
template<>
template<>
void testObj::test<2>(void)
{
  Singleton::get()->loggerConfig();
}

// test getting persistency's config
template<>
template<>
void testObj::test<3>(void)
{
  Singleton::get()->persistencyConfig();
}

// test getting filters' config
template<>
template<>
void testObj::test<4>(void)
{
  Singleton::get()->filtersConfig();
}

} // namespace tut
