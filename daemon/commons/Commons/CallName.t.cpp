/*
 * CallName.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Commons/CallName.h"


namespace
{
struct CallNameTestClass
{
};

typedef CallNameTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/CallName");
} // unnamed namespace


namespace tut
{

// check if macro compiles and generates const char*
template<>
template<>
void testObj::test<1>(void)
{
  const char *c=CALLNAME;
  ensure("NULL pointer recieved", c!=NULL);
}

// test content
template<>
template<>
void testObj::test<2>(void)
{
  const char *c=CALLNAME;
  ensure("expected string not found", strstr(c, "::test")!=NULL );
}

} // namespace tut
