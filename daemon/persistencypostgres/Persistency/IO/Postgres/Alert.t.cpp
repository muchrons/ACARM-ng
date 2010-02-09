/*
 * Alert.t.cpp
 *
 */

#include <tut.h>

//TODO: tests

using namespace std;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Alert");
} //unnamned namespace


namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{

}

} // namespace tut

