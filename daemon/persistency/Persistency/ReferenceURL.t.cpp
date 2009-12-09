/*
 * ReferenceURL.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/ReferenceURL.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
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

} // namespace tut
