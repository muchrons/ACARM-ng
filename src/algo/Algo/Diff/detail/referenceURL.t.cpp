/*
 * referenceURL.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/referenceURL.hpp"

using namespace Persistency;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/referenceURL");
} // unnamed namespace


namespace tut
{

// check if identical reference URLs are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( ReferenceURL("name", "http://et.home"), ReferenceURL("name", "http://et.home") );
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different referenceURLs
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( ReferenceURL("other name", "http://et.home"), ReferenceURL("name", "http://eat.home") );
  ensure("elements differ too little", s.get()>0.5);
  ensure("elements marked identical", s.get()<1);
}

// test totally different referenceURLs
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=compare( ReferenceURL("other stuff", "http://baszerr.org"), ReferenceURL("name", "ftp://eat.home") );
  ensure("elements differ too much", s.get()<0.1);
}

} // namespace tut
