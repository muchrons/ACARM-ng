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
const char *defaultFile="test_config.xml";  // TODO: hardcoded value

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

// test rereading configuration from default file.
template<>
template<>
void testObj::test<5>(void)
{
  const PersistencyConfig *tmp=&Singleton::get()->persistencyConfig();
  Singleton::get()->rereadConfig();             // re-read config
  const PersistencyConfig *ptr=&Singleton::get()->persistencyConfig();
  ensure("configuration not changed", ptr!=tmp);
}

// test reading different configuration file
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("invalid initial read",
                Singleton::get()->persistencyConfig().getType(),
                "postgres");

  Singleton::get()->rereadConfig("testdata/other_config.xml");  // re-read config

  ensure_equals("invalid initial read",
                Singleton::get()->persistencyConfig().getType(),
                "stubs");
}

// test if configuration stays un-touched in case of error
template<>
template<>
void testObj::test<7>(void)
{
  const PersistencyConfig *tmp=&Singleton::get()->persistencyConfig();
  try
  {
    Singleton::get()->rereadConfig("testdata/invalid_defaul_appender.xml");
    fail("rereading configuration didn't failed on invalid config");
  }
  catch(const Exception &)
  {
    // this is expected
  }
  const PersistencyConfig *ptr=&Singleton::get()->persistencyConfig();
  ensure("configuration changed", ptr==tmp);
}

// test getting triggers' config
template<>
template<>
void testObj::test<8>(void)
{
  Singleton::get()->triggersConfig();
}

// test getting inputs' config
template<>
template<>
void testObj::test<9>(void)
{
  Singleton::get()->inputsConfig();
}

// test getting preprocessor's config
template<>
template<>
void testObj::test<10>(void)
{
  Singleton::get()->preprocessorConfig();
}

// test getting general config
template<>
template<>
void testObj::test<11>(void)
{
  Singleton::get()->generalConfig();
}

// test throw on reading non-existing file
template<>
template<>
void testObj::test<12>(void)
{
  try
  {
    Singleton::get()->rereadConfig("file/that/does/not/exist");
    fail("rereading non-exisitng file didn't failed");
  }
  catch(const ExceptionFileAccessError &)
  {
    // this is expected
  }
}

} // namespace tut
