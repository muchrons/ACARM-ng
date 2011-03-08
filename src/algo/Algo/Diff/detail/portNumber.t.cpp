/*
 * portNumber.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/portNumber.hpp"

using namespace Persistency;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/portNumber");
} // unnamed namespace


namespace tut
{

// check if identical portNumbers are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( PortNumber(42), PortNumber(42) );
  ensure_equals("identical elements differ", s.get(), 1);
}

// test different portNumbers
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( PortNumber(42), PortNumber(43) );
  ensure_equals("different elements do not differ", s.get(), 0);
}

} // namespace tut
