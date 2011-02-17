/*
 * Entry.t.cpp
 *
 */

#include <tut.h>


namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/Entry");
} // unnamed namespace



namespace tut
{

// TODO
template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
