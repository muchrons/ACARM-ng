/*
 * Similarity.t.cpp
 *
 */
#include <tut.h>
#include <limits>

#include "Algo/Diff/Similarity.hpp"

using namespace Algo::Diff;

namespace
{

struct TestClass
{
  void testThrow(const double v) const
  {
    try
    {
      Similarity s(v);
      tut::fail("c-tor didn't throw on error");
    }
    catch(const Similarity::ExceptionInvalidValue &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/Similarity");
} // unnamed namespace


namespace tut
{

// check normal construction
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s(0.42);
  ensure_equals("invalid value", s.get(), 0.42);
}

// check exception when avlue is too small
template<>
template<>
void testObj::test<2>(void)
{
  testThrow(-0.1);
}

// check exception when avlue is too big
template<>
template<>
void testObj::test<3>(void)
{
  testThrow(1.1);
}

// test minimal value
template<>
template<>
void testObj::test<4>(void)
{
  const Similarity s(0);
  ensure_equals("invalid value", s.get(), 0);
}

// test maximal value
template<>
template<>
void testObj::test<5>(void)
{
  const Similarity s(1);
  ensure_equals("invalid value", s.get(), 1);
}

// test named c-tor
template<>
template<>
void testObj::test<6>(void)
{
  const Similarity s=Similarity::normalize(0.1);
  ensure_equals("invalid value", s.get(), 0.1);
}

// test too small value
template<>
template<>
void testObj::test<7>(void)
{
  const Similarity s=Similarity::normalize(-0.1);
  ensure_equals("invalid value", s.get(), 0);
}

// test too big value
template<>
template<>
void testObj::test<8>(void)
{
  const Similarity s=Similarity::normalize(1.1);
  ensure_equals("invalid value", s.get(), 1);
}

// test passing infinity
template<>
template<>
void testObj::test<9>(void)
{
  testThrow( std::numeric_limits<double>::infinity() );
}

// test passing NaN
template<>
template<>
void testObj::test<10>(void)
{
  if( !std::numeric_limits<double>::has_quiet_NaN )
    fail("no quiet NaN support - test will not run");
  testThrow( std::numeric_limits<double>::quiet_NaN() );
}

} // namespace tut
