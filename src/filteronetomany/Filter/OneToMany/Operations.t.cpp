/*
 * Operations.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Filter/OneToMany/Operations.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::OneToMany;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/OneToMany/Operations");
} // unnamed namespace


namespace tut
{

// test getting meta alert name from host with dns name
template<>
template<>
void testObj::test<1>(void)
{
  const string n=Operations::getMetaAlertName( makeNewHost() ).get();
  ensure_equals("invlaid name", n, "[one2many] multiple attacks from host 1.2.3.4 (dns.org)");
}

// test getting meta alert name from host without dns name
template<>
template<>
void testObj::test<2>(void)
{
  HostPtrNN h( makeNewHost("1.2.3.4", NULL) );
  const string n=Operations::getMetaAlertName(h).get();
  ensure_equals("invlaid name", n, "[one2many] multiple attacks from host 1.2.3.4");
}

// getting repoted hosts smoke test
template<>
template<>
void testObj::test<3>(void)
{
  Operations::getHostsArray( makeNewLeaf() );
}

} // namespace tut
