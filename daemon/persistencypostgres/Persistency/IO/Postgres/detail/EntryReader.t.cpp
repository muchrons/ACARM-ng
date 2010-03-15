/*
 * EntryReader.t.cpp
 *
 */

#include <tut.h>

#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
namespace
{
struct TestClass
{
  TestClass(void)
  {
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntryReader");

} // unnamed namespace
namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
  //TODO
}

} // namespace tut
