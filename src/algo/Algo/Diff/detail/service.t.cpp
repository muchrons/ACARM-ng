/*
 * service.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/service.hpp"

#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace ::Persistency;
using namespace Algo::Diff;
using namespace TestHelpers::Persistency;

namespace
{

typedef Algo::Diff::detail::Comparer<const Persistency::Service> Cmp;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/service");
} // unnamed namespace


namespace tut
{

// check if identical services are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Service s1("name", 42, "prot", makeNewReferenceURL().shared_ptr() );
  const Service s2("name", 42, "prot", makeNewReferenceURL().shared_ptr() );
  const Similarity s=Cmp::cmp(s1, s2);
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different services
template<>
template<>
void testObj::test<2>(void)
{
  const Service s1("name",  42, "prot", makeNewReferenceURL("abc").shared_ptr() );
  const Service s2("name2", 43, "prot", makeNewReferenceURL("aBC").shared_ptr() );
  const Similarity s=Cmp::cmp(s1, s2);
  ensure("elements differ too little", s.get()>0.5);
  ensure("elements marked identical", s.get()<1);
}

// test totally different services
template<>
template<>
void testObj::test<3>(void)
{
  const Service s1("abc", 43, "def", makeNewReferenceURL("bbb").shared_ptr() );
  const Service s2("xyz", 42, "qwe", makeNewReferenceURL("aaa").shared_ptr() );
  const Similarity s=Cmp::cmp(s1, s2);
  ensure("elements differ too much", s.get()<0.1);
}

} // namespace tut
