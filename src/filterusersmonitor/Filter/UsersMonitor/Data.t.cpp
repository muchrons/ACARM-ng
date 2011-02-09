/*
 * Data.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/UsersMonitor/Data.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void)
  {
  }

  ConstAlertPtrNN mkAlert(const char *src1=NULL, const char *src2=NULL,
                          const char *dst1=NULL, const char *dst2=NULL) const
  {
  }

  HostPtrNN mkHost(const char *user1, const char *user2) const
  {
  }

  ProcessPtrNN mkProc(const char *user) const
  {
    return ProcessPtrNN( new Process("/a/b", "b", NULL, NULL, NULL, user, NULL, ConstReferenceURLPtr()) );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/UsersMonitor/Data");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
