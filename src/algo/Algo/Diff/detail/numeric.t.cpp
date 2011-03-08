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
  template<typename T>
  Similarity cmp(const T e1, const T e2) const
  {
    return Algo::Diff::detail::Comparer<const T>::cmp(e1, e2);
  }
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
  const Similarity s=cmp<int>(10, 10);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=cmp(10, 20);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=cmp<long>(10, 10);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<4>(void)
{
  const Similarity s=cmp<long>(10, 20);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<5>(void)
{
  const Similarity s=cmp<unsigned long>(10, 10);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<6>(void)
{
  const Similarity s=cmp<unsigned long>(10, 20);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

// check if identical numerics are the same
template<>
template<>
void testObj::test<7>(void)
{
  const Similarity s=cmp<pid_t>(10, 10);
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different numerics
template<>
template<>
void testObj::test<8>(void)
{
  const Similarity s=cmp<pid_t>(10, 20);
  ensure_equals("different elements reported non-different", s.get(), 0);
}

} // namespace tut
