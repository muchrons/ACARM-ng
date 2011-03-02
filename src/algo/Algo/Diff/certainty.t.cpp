/*
 * certainty.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/certainty.hpp"

using namespace Persistency;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/certainty");
} // unnamed namespace


namespace tut
{

// check if identical certainties are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( Certainty(0.5), Certainty(0.5) );
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different certainties
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( Certainty(0.5), Certainty(0.4) );
  ensure("elements differ too much", s.get()>0.75);
  ensure("elements marked identical", s.get()<1);
}

// test totally different certainties
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=compare( Certainty(0.4), Certainty(0.9) );
  ensure("elements differ too little", s.get()<0.01);
}

} // namespace tut
