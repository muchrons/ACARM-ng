/*
 * md5sum.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/md5sum.hpp"

using namespace Persistency;
using namespace Algo::Diff;

namespace
{

typedef Algo::Diff::detail::Comparer<const Persistency::MD5Sum> Cmp;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/md5sum");
} // unnamed namespace


namespace tut
{

// check if identical md5sums are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=Cmp::cmp( MD5Sum::createFromString("01234567890123456789012345678901"),
                               MD5Sum::createFromString("01234567890123456789012345678901") );
  ensure_equals("identical elements differ", s.get(), 1);
}

// test totally different md5sums
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=Cmp::cmp( MD5Sum::createFromString("01234567890123456789012345678aaa"),
                               MD5Sum::createFromString("01234567890123456789012345678901") );
  ensure_equals("different elements do not differ", s.get(), 0);
}

} // namespace tut
