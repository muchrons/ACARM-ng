/*
 * ReferenceURL.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/ReferenceURL.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/ReferenceURL");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const char         *url="http://www.bugs.org/123/456";
  const ReferenceURL  ti("some bug", url);
  ensure_equals("invalid name", ti.getName().get(), string("some bug") );
  ensure_equals("invalid URL",  ti.getURL().get(),  string(url)        );
}

// test comparison, when differ only in URL
template<>
template<>
void testObj::test<2>(void)
{
  const char         *url ="http://your.org";
  const char         *name="xyz";
  const ReferenceURL  ti1(name, url);
  const ReferenceURL  ti2(name, "http://my.org");
  TestHelpers::checkEquality(ti1, ti2);
}

// test comparison, when differ only in name
template<>
template<>
void testObj::test<3>(void)
{
  const char         *url ="http://your.org";
  const char         *name="xyz";
  const ReferenceURL  ti1("ab", url);
  const ReferenceURL  ti2(name, url);
  TestHelpers::checkEquality(ti1, ti2);
}

} // namespace tut
