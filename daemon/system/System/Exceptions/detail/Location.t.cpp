/*
 * Location.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/Exceptions/detail/Location.hpp"

using namespace System::Exceptions::detail;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Exceptions/detail/Location");
} // unnamed namespace


namespace tut
{

// test creating object
template<>
template<>
void testObj::test<1>(void)
{
  const Location     l("file.txt", 42, "funcxyz()");
  const std::string &str=l.getStr();
  ensure_equals("invalid string generated", str, "file.txt:42@funcxyz()");
}

// test copyablility
template<>
template<>
void testObj::test<2>(void)
{
  Location l1("file1.txt", 41, "funcxyz1()");
  Location l2("file2.txt", 42, "funcxyz2()");
  l2=l1;
  const std::string &str=l1.getStr();
  ensure_equals("invalid string generated", str, "file1.txt:41@funcxyz1()");
}

} // namespace tut
