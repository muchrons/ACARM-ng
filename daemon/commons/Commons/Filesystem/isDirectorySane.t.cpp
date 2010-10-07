/*
 * isDirectorySane.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  template<typename TEx>
  void ensureThrow(const path &p) const
  {
    try
    {
      isDirectorySane(p);   // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/isDirectorySane");
} // unnamed namespace


namespace tut
{

// test if saint dir is marked as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("normal dir not sain", isDirectorySane("testdata") );
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

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

} // namespace tut
