/*
 * IsCollection.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/IsCollection.hpp"

using namespace Persistency::Facades::StrAccess;
#if 0               // TODO

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/IsCollection");
} // unnamed namespace


namespace tut
{

// test non-pointer
template<>
template<>
void testObj::test<1>(void)
{
  ensure("got false positive", IsCollection<int>::value==false);
}

// test vector
template<>
template<>
void testObj::test<2>(void)
{
  ensure("vector not recognized", IsCollection< std::vector<int> >::value);
}

// test grwoingvector
template<>
template<>
void testObj::test<3>(void)
{
  ensure("growing vector not recognized", IsCollection< Base::Threads::GrowingVector<int> >::value);
}

// test nonemptyvector
template<>
template<>
void testObj::test<4>(void)
{
  ensure("non-empty vector not recognized", IsCollection< Base::NonEmptyVector<int> >::value);
}

// test nodechildrenvector
template<>
template<>
void testObj::test<5>(void)
{
  ensure("node children vector not recognized", IsCollection<Persistency::NodeChildrenVector>::value);
}

} // namespace tut
#endif                      
