/*
 * Strategy.t.cpp
 *
 */

namespace
{
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/Strategy");
} // unnamed namespace



namespace tut
{

// test if entry is not interesting when no target host is set
template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
