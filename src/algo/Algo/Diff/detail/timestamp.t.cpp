/*
 * timestamp.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/timestamp.hpp"

using namespace Persistency;
using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/timestamp");
} // unnamed namespace


namespace tut
{

// check if identical timestamps are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( Timestamp(42), Timestamp(42) );
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different timestamps
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( Timestamp(40), Timestamp(50) );
  ensure("elements differ too little", s.get()>0.75);
  ensure("elements marked identical", s.get()<1);
}

// test totally different timestamps
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=compare( Timestamp(1234567), Timestamp(50) );
  ensure("elements differ too much", s.get()<0.01);
}

} // namespace tut
