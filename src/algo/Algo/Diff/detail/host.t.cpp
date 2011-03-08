/*
 * host.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/host.hpp"

#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace ::Persistency;
using namespace Algo::Diff;
using namespace TestHelpers::Persistency;

namespace
{

typedef Algo::Diff::detail::Comparer<const Persistency::Host> Cmp;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/host");
} // unnamed namespace


namespace tut
{

// check if identical hosts are the same
template<>
template<>
void testObj::test<1>(void)
{
  const ServicePtrNN    srv( new Service("name", 42, "prot", makeNewReferenceURL().shared_ptr()) );
  const Host::Services  srvs(1, srv);
  const ProcessPtrNN    proc( new Process("/other/dir", "x", NULL, NULL, NULL, "stefan", "xyz", ReferenceURLPtr()) );
  const Host::Processes procs(1, proc);
  const Host::IP        ip  =Host::IP::from_string("10.20.30.40");
  const Host::Netmask   mask=Host::Netmask::from_string("255.255.0.0");

  const Host            h1(ip, &mask, "Linux", ReferenceURLPtr(), srvs, procs, "myname");
  const Host            h2(ip, &mask, "Linux", ReferenceURLPtr(), srvs, procs, "myname");
  const Similarity      s=Cmp::cmp(h1, h2);
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different hosts
template<>
template<>
void testObj::test<2>(void)
{
  const Host::IP   ip  =Host::IP::from_string("10.20.30.40");
  const Host       h1(ip, NULL, "Linux 2", ReferenceURLPtr(), Host::Services(), Host::Processes(), "myname");
  const Host       h2(ip, NULL, "Linux 1", ReferenceURLPtr(), Host::Services(), Host::Processes(), "myname2");
  const Similarity s=Cmp::cmp(h1, h2);
  ensure("elements differ too much", s.get()>0.75);
  ensure("elements does not differ", s.get()<1);
}

// test totally different hosts
template<>
template<>
void testObj::test<3>(void)
{
  const ServicePtrNN    srv( new Service("name", 42, "prot", makeNewReferenceURL().shared_ptr()) );
  const Host::Services  srvs(1, srv);
  const ProcessPtrNN    proc( new Process("/other/dir", "x", NULL, NULL, NULL, "stefan", "xyz", ReferenceURLPtr()) );
  const Host::Processes procs(1, proc);
  const Host::IP        ip1 =Host::IP::from_string("10.0.0.10");
  const Host::IP        ip2 =Host::IP::from_string("90.90.90.90");
  const Host::Netmask   mask=Host::Netmask::from_string("255.255.0.0");
  const ReferenceURLPtr url =makeNewReferenceURL().shared_ptr();

  const Host            h1(ip1, &mask, "Linux",    ReferenceURLPtr(), srvs,             Host::Processes(), "myname");
  const Host            h2(ip2, NULL,  "Winblows", url,               Host::Services(), procs,             "other one");
  const Similarity      s=Cmp::cmp(h1, h2);
  ensure("elements differ too much", s.get()<0.1);
}

} // namespace tut
