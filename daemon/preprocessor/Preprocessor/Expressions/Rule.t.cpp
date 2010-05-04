/*
 * Rule.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Rule.hpp"
#include "Preprocessor/Expressions/TestsCommon.t.hpp"

using Persistency::Alert;
using Preprocessor::Path;
using namespace TestHelpers::Persistency;
using namespace Preprocessor::Expressions;

namespace
{
struct TestClass
{
  TestClass(void):
    alert_("alert name",
           Alert::SourceAnalyzers( makeAnalyzer() ),
           ::Base::NullValue<Persistency::Timestamp>( Persistency::Timestamp(123) ).get(),
           Persistency::Timestamp(12345),
           Persistency::Severity(Persistency::SeverityLevel::INFO),
           Persistency::Certainty(0.42),
           "some test alert",
           makeHosts("1.1.1.1", "dns.org",
                     "2.2.2.2", "kszy.net"),
           makeHosts("3.2.2.3", "ogre.org",
                     "3.2.2.3", "ogre.org") )
  {
  }

  Persistency::AnalyzerPtrNN makeAnalyzer(void) const
  {
    const Persistency::Analyzer::IP ip=Persistency::Host::IPv4::from_string("6.6.6.0");
    return Persistency::AnalyzerPtrNN( new Persistency::Analyzer("analyzer",
                                                                 "v1.2.3",
                                                                 "Linux!",
                                                                 &ip) );
  }

  Alert::ReportedHosts makeHosts(const char *ip1, const char *dns1,
                                 const char *ip2, const char *dns2) const
  {
    Alert::ReportedHosts hosts;
    hosts.push_back( makeHost(ip1, dns1) );
    hosts.push_back( makeHost(ip2, dns2) );
    return hosts;
  }

  Persistency::HostPtrNN makeHost(const char *ip, const char *dns) const
  {
    const Persistency::Host::Netmask_v4  mask(mask4_bytes);
    Persistency::Host::ReportedServices  services;
    Persistency::Host::ReportedProcesses procs;
    return Persistency::HostPtrNN( new Persistency::Host( Persistency::Host::IPv4::from_string(ip),
                                                          &mask,
                                                          "penguin",
                                                          makeNewReferenceURL(),
                                                          services,
                                                          procs,
                                                          dns) );
  }

  const Alert alert_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Expressions/Rule");
} // unnamed namespace


namespace tut
{

// test Mode::EQUALS - positive case
template<>
template<>
void testObj::test<1>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::EQUALS, "alert name" );
  ensure("equals failed", r.compute(alert_)==true );
}

// test Mode::EQUALS - negative case
template<>
template<>
void testObj::test<2>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::EQUALS, "some alert that differs" );
  ensure("equals didn't failed", r.compute(alert_)==false );
}

// test Mode::CONTAINS - positive case
template<>
template<>
void testObj::test<3>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::CONTAINS, "alert" );
  ensure("contains failed", r.compute(alert_)==true );
}

// test Mode::CONTAINS - negative case
template<>
template<>
void testObj::test<4>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::CONTAINS, "ALERT" );
  ensure("contains didn't failed", r.compute(alert_)==false );
}

// test detection time
template<>
template<>
void testObj::test<5>(void)
{
  const Rule r( Path("alert.detected"), Rule::Mode::EQUALS, "123" );
  ensure("invalid detection time", r.compute(alert_)==true );
}

// test creation time
template<>
template<>
void testObj::test<6>(void)
{
  const Rule r( Path("alert.created"), Rule::Mode::EQUALS, "12345" );
  ensure("invalid creation time", r.compute(alert_)==true );
}

// test certanity
template<>
template<>
void testObj::test<7>(void)
{
  const Rule r( Path("alert.certainty"), Rule::Mode::CONTAINS, "0.4" );
  ensure("invalid certanity", r.compute(alert_)==true );
}

// test severity
template<>
template<>
void testObj::test<8>(void)
{
  const Rule r( Path("alert.severity"), Rule::Mode::EQUALS, "info" );
  ensure("invalid severity", r.compute(alert_)==true );
}

// test description
template<>
template<>
void testObj::test<9>(void)
{
  const Rule r( Path("alert.description"), Rule::Mode::EQUALS, "some test alert" );
  ensure("invalid description", r.compute(alert_)==true );
}

// test analyzer.name
template<>
template<>
void testObj::test<10>(void)
{
  const Rule r( Path("alert.analyzers.*.name"), Rule::Mode::EQUALS, "analyzer" );
  ensure("invalid analyzer's name", r.compute(alert_)==true );
}

// test analyzer.version
template<>
template<>
void testObj::test<11>(void)
{
  const Rule r( Path("alert.analyzers.*.version"), Rule::Mode::EQUALS, "v1.2.3" );
  ensure("invalid analyzer's version", r.compute(alert_)==true );
}

// test analyzer.os
template<>
template<>
void testObj::test<12>(void)
{
  const Rule r( Path("alert.analyzers.*.os"), Rule::Mode::EQUALS, "Linux!" );
  ensure("invalid analyzer's OS", r.compute(alert_)==true );
}

// test analyzer.ip
template<>
template<>
void testObj::test<13>(void)
{
  const Rule r( Path("alert.analyzers.*.ip"), Rule::Mode::EQUALS, "6.6.6.0" );
  ensure("invalid analyzer's IP", r.compute(alert_)==true );
}

// test host.ip
template<>
template<>
void testObj::test<14>(void)
{
  const Rule r( Path("alert.source.$.ip"), Rule::Mode::EQUALS, "2.2.2.2" );
  ensure("invalid host's IP", r.compute(alert_)==true );
}

// test NULL value
template<>
template<>
void testObj::test<15>(void)
{
  const Rule r( Path("alert.detected"), Rule::Mode::EQUALS, "<NULL>" );
  ensure("invalid NULL value", r.compute( *makeAlert() )==true );
}

// test host.mask
template<>
template<>
void testObj::test<16>(void)
{
  const Rule r( Path("alert.source.*.mask"), Rule::Mode::EQUALS, "255.255.0.0" );
  ensure("invalid host's mask", r.compute(alert_)==true );
}

// host.os
template<>
template<>
void testObj::test<17>(void)
{
  const Rule r( Path("alert.source.*.os"), Rule::Mode::EQUALS, "penguin" );
  ensure("invalid mask's OS", r.compute(alert_)==true );
}

// 
template<>
template<>
void testObj::test<18>(void)
{
}

// 
template<>
template<>
void testObj::test<19>(void)
{
}

// 
template<>
template<>
void testObj::test<20>(void)
{
}

// 
template<>
template<>
void testObj::test<21>(void)
{
}

// 
template<>
template<>
void testObj::test<22>(void)
{
}

// 
template<>
template<>
void testObj::test<23>(void)
{
}

// 
template<>
template<>
void testObj::test<24>(void)
{
}

// 
template<>
template<>
void testObj::test<25>(void)
{
}

// 
template<>
template<>
void testObj::test<26>(void)
{
}

// 
template<>
template<>
void testObj::test<27>(void)
{
}

// 
template<>
template<>
void testObj::test<28>(void)
{
}

// 
template<>
template<>
void testObj::test<29>(void)
{
}

// 
template<>
template<>
void testObj::test<30>(void)
{
}

// 
template<>
template<>
void testObj::test<31>(void)
{
}

// 
template<>
template<>
void testObj::test<32>(void)
{
}

// 
template<>
template<>
void testObj::test<33>(void)
{
}

// 
template<>
template<>
void testObj::test<34>(void)
{
}

// 
template<>
template<>
void testObj::test<35>(void)
{
}

// 
template<>
template<>
void testObj::test<36>(void)
{
}

// 
template<>
template<>
void testObj::test<37>(void)
{
}

// 
template<>
template<>
void testObj::test<38>(void)
{
}

// 
template<>
template<>
void testObj::test<39>(void)
{
}

// 
template<>
template<>
void testObj::test<40>(void)
{
}

// 
template<>
template<>
void testObj::test<41>(void)
{
}

// 
template<>
template<>
void testObj::test<42>(void)
{
}

// 
template<>
template<>
void testObj::test<43>(void)
{
}

// 
template<>
template<>
void testObj::test<44>(void)
{
}

// 
template<>
template<>
void testObj::test<45>(void)
{
}

// 
template<>
template<>
void testObj::test<46>(void)
{
}

// 
template<>
template<>
void testObj::test<47>(void)
{
}

// 
template<>
template<>
void testObj::test<48>(void)
{
}

// TODO: finish these tests

} // namespace tut
