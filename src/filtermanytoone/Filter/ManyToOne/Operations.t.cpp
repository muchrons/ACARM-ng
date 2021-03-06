/*
 * Operations.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Filter/ManyToOne/Operations.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::ManyToOne;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/ManyToOne/Operations");
} // unnamed namespace


namespace tut
{

// test getting meta alert name from host with dns name
template<>
template<>
void testObj::test<1>(void)
{
  const string n=Operations::getMetaAlertName( makeNewHost() ).get();
  ensure_equals("invlaid name", n, "[many2one] multiple attacks on host 1.2.3.4 (dns.org)");
}

// test getting meta alert name from host without dns name
template<>
template<>
void testObj::test<2>(void)
{
  HostPtrNN    h( new Host( Host::IPv4::from_string("1.2.3.4"),
                            NULL,
                            "myos",
                            makeNewReferenceURL().shared_ptr(),
                            Host::Services(),
                            Host::Processes(),
                            NULL ) );
  const string n=Operations::getMetaAlertName(h).get();
  ensure_equals("invlaid name", n, "[many2one] multiple attacks on host 1.2.3.4");
}

// getting repoted hosts smoke test
template<>
template<>
void testObj::test<3>(void)
{
  Operations::getHostsArray( makeNewLeaf() );
}

} // namespace tut
