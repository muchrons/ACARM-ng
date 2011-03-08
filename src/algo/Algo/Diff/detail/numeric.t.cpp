/*
 * numeric.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/numeric.hpp"

using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/numeric");
} // unnamed namespace


namespace tut
{

// check if identical numerics are the same
template<>
template<>
void testObj::test<1>(void)
{
  const int        v1=10;
  const int        v2=10;
  const Similarity s =compare(v1, v2);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<2>(void)
{
  const int        v1=10;
  const int        v2=20;
  const Similarity s =compare(v1, v2);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<3>(void)
{
  const long       v1=10;
  const long       v2=10;
  const Similarity s =compare(v1, v2);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<4>(void)
{
  const long       v1=10;
  const long       v2=20;
  const Similarity s =compare(v1, v2);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<5>(void)
{
  const unsigned long v1=10;
  const unsigned long v2=10;
  const Similarity    s =compare(v1, v2);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<6>(void)
{
  const unsigned long v1=10;
  const unsigned long v2=20;
  const Similarity    s =compare(v1, v2);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<7>(void)
{
  const pid_t      v1=10;
  const pid_t      v2=10;
  const Similarity s=compare(v1, v2);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<8>(void)
{
  const pid_t      v1=10;
  const pid_t      v2=20;
  const Similarity s=compare(v1, v2);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

} // namespace tut
