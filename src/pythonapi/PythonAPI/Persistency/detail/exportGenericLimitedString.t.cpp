/*
 * exportGenericLimitedString.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Persistency/detail/exportGenericLimitedString.hpp"

using namespace std;

namespace
{

// simple stub that matches required interface
struct TestStringClass
{
  TestStringClass(void)
  {
  }
  TestStringClass(const char *)
  {
  }
  TestStringClass(const std::string &)
  {
  }
  const char *get(void) const
  {
    return NULL;
  }
};

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/detail/exportGenericLimitedString");
} // unnamed namespace

namespace tut
{

// try preparing test string for export
template<>
template<>
void testObj::test<1>(void)
{
  PythonAPI::Persistency::detail::exportGenericLimitedString<TestStringClass>("test");
}

} // namespace tut
