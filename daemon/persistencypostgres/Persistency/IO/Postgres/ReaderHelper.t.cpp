/*
 * ReaderHelper.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/ReaderHelper.hpp"
namespace
{

struct TestClass
{

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ReaderHelper");
} //unnamned namespace

namespace tut
{

// trying ...
template<>
template<>
void testObj::test<1>(void)
{

}

} // namespace tut
