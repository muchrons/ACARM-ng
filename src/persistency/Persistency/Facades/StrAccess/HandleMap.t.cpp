/*
 * HandleMap.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/HandleMap.hpp"

using namespace std;
using namespace boost::mpl;
using namespace Persistency::Facades::StrAccess;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/HandleMap");
} // unnamed namespace


namespace tut
{

// test term
template<>
template<>
void testObj::test<1>(void)
{
  const string str=at<HandleMap,OnTerm>::type::get("test");
  ensure_equals("invalid term handle", str, "test");
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
