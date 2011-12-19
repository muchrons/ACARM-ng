/*
 * Rule.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Rule.hpp"
#include "Preprocessor/Expressions/TestsCommon.t.hpp"

using Base::NullValue;
using Persistency::Alert;
using namespace TestHelpers::Persistency;
using namespace Preprocessor::Expressions;


namespace
{
struct TestClass
{
  TestClass(void):
    alert_(new Alert("alert name",
                     Alert::Analyzers( makeAnalyzer() ),
                     NullValue<Persistency::Timestamp>( Persistency::Timestamp(123) ).get(),
                     Persistency::Timestamp(12345),
                     Persistency::Severity(Persistency::SeverityLevel::INFO),
                     Persistency::Certainty(0.42),
                     "some test alert",
                     makeHosts("1.1.1.1", "dns.org",
                               "2.2.2.2", "kszy.net"),
                     makeHosts("3.2.2.3", "ogre.org",
                               "3.2.2.3", "ogre.org") )),
    leaf_( makeNewLeaf(alert_) ),
    fmt_( makeFormatterConfig() )
  {
    assert( leaf_->isLeaf() );
  }

  Persistency::AnalyzerPtrNN makeAnalyzer(void) const
  {
    const Persistency::Analyzer::IP ip=Persistency::Host::IPv4::from_string("6.6.6.0");
    return Persistency::AnalyzerPtrNN( new Persistency::Analyzer(42u,
                                                                 "analyzer",
                                                                 "v1.2.3",
                                                                 "Linux!",
                                                                 &ip) );
  }

  Alert::Hosts makeHosts(const char *ip1, const char *dns1,
                         const char *ip2, const char *dns2) const
  {
    Alert::Hosts hosts;
    hosts.push_back( makeHost(ip1, dns1) );
    hosts.push_back( makeHost(ip2, dns2) );
    return hosts;
  }

  Persistency::HostPtrNN makeHost(const char *ip, const char *dns) const
  {
    const Persistency::Host::Netmask_v4  mask(mask4_bytes);
    Persistency::Host::Services  services;
    services.push_back( makeService() );
    Persistency::Host::Processes procs;
    procs.push_back( makeProcess() );
    return Persistency::HostPtrNN( new Persistency::Host( Persistency::Host::IPv4::from_string(ip),
                                                          &mask,
                                                          "penguin",
                                                          makeNewReferenceURL().shared_ptr(),
                                                          services,
                                                          procs,
                                                          dns) );
  }

  Persistency::ProcessPtrNN makeProcess(void) const
  {
    const Persistency::MD5Sum md5=Persistency::MD5Sum::createFromString("01234567890123456789012345678901");
    const pid_t               pid=666;
    const int                 uid=42;
    return Persistency::ProcessPtrNN( new Persistency::Process(  "/path/to/bin",
                                                                 "name",
                                                                &md5,
                                                                &pid,
                                                                &uid,
                                                                 "looser",
                                                                 "-a -b -c",
                                                                 makeNewReferenceURL().shared_ptr() ) );
  }

  Persistency::ServicePtrNN makeService(void) const
  {
    return Persistency::ServicePtrNN( new Persistency::Service( "service name",
                                                                42,
                                                                "magic_proto",
                                                                makeNewReferenceURL().shared_ptr() ) );
  }

  ConfigIO::Preprocessor::FormatterConfig makeFormatterConfig(void) const
  {
    // TODO: this is ugly as hell - it uses detail implementation of the base component. fix it...
    ConfigIO::Preprocessor::detail::FormatterConfigData v;
    v.type_=ConfigIO::Preprocessor::detail::FormatterConfigData::VALUE;
    return ConfigIO::Preprocessor::FormatterConfig(v);
  }

  Persistency::AlertPtrNN                 alert_;
  Persistency::ConstGraphNodePtrNN        leaf_;
  ConfigIO::Preprocessor::FormatterConfig fmt_;
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
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::EQUALS, "alert name" , fmt_);
  ensure("equals failed", r.compute(leaf_)==true );
}

// test Mode::EQUALS - negative case
template<>
template<>
void testObj::test<2>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::EQUALS, "some alert that differs", fmt_ );
  ensure("equals didn't failed", r.compute(leaf_)==false );
}

// test Mode::CONTAINS - positive case
template<>
template<>
void testObj::test<3>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::CONTAINS, "alert", fmt_ );
  ensure("contains failed", r.compute(leaf_)==true );
}

// test Mode::CONTAINS - negative case
template<>
template<>
void testObj::test<4>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::CONTAINS, "ALERT", fmt_ );
  ensure("contains didn't failed", r.compute(leaf_)==false );
}

// test detection time
template<>
template<>
void testObj::test<5>(void)
{
  const Rule r( Rule::Path("metaalert.alert.detected"), Rule::Mode::EQUALS, "123", fmt_ );
  ensure("invalid detection time", r.compute(leaf_)==true );
}

// test creation time
template<>
template<>
void testObj::test<6>(void)
{
  const Rule r( Rule::Path("metaalert.alert.created"), Rule::Mode::EQUALS, "12345", fmt_ );
  ensure("invalid creation time", r.compute(leaf_)==true );
}

// test certainty
template<>
template<>
void testObj::test<7>(void)
{
  const Rule r( Rule::Path("metaalert.alert.certainty"), Rule::Mode::CONTAINS, "0.4", fmt_ );
  ensure("invalid certainty", r.compute(leaf_)==true );
}

// test severity
template<>
template<>
void testObj::test<8>(void)
{
  const Rule r( Rule::Path("metaalert.alert.severity"), Rule::Mode::EQUALS, "info", fmt_ );
  ensure("invalid severity", r.compute(leaf_)==true );
}

// test description
template<>
template<>
void testObj::test<9>(void)
{
  const Rule r( Rule::Path("metaalert.alert.description"), Rule::Mode::EQUALS, "some test alert", fmt_ );
  ensure("invalid description", r.compute(leaf_)==true );
}

// test analyzer.name
template<>
template<>
void testObj::test<10>(void)
{
  const Rule r( Rule::Path("metaalert.alert.analyzers.*.name"), Rule::Mode::EQUALS, "analyzer", fmt_ );
  ensure("invalid analyzer's name", r.compute(leaf_)==true );
}

// test analyzer.version
template<>
template<>
void testObj::test<11>(void)
{
  const Rule r( Rule::Path("metaalert.alert.analyzers.*.version"), Rule::Mode::EQUALS, "v1.2.3", fmt_ );
  ensure("invalid analyzer's version", r.compute(leaf_)==true );
}

// test analyzer.os
template<>
template<>
void testObj::test<12>(void)
{
  const Rule r( Rule::Path("metaalert.alert.analyzers.*.os"), Rule::Mode::EQUALS, "Linux!", fmt_ );
  ensure("invalid analyzer's OS", r.compute(leaf_)==true );
}

// test analyzer.ip
template<>
template<>
void testObj::test<13>(void)
{
  const Rule r( Rule::Path("metaalert.alert.analyzers.*.ip"), Rule::Mode::EQUALS, "6.6.6.0", fmt_ );
  ensure("invalid analyzer's IP", r.compute(leaf_)==true );
}

// test host.ip
template<>
template<>
void testObj::test<14>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.$.ip"), Rule::Mode::EQUALS, "2.2.2.2", fmt_ );
  ensure("invalid host's IP", r.compute(leaf_)==true );
}

// test NULL value
template<>
template<>
void testObj::test<15>(void)
{
  const Rule r( Rule::Path("metaalert.alert.detected"), Rule::Mode::EQUALS, "<NULL>", fmt_ );
  ensure("invalid NULL value", r.compute( makeAlert() )==true );
}

// test host.mask
template<>
template<>
void testObj::test<16>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.netmask"), Rule::Mode::EQUALS, "255.255.0.0", fmt_ );
  ensure("invalid host's mask", r.compute(leaf_)==true );
}

// host.os
template<>
template<>
void testObj::test<17>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.os"), Rule::Mode::EQUALS, "penguin", fmt_ );
  ensure("invalid mask's OS", r.compute(leaf_)==true );
}

// test url.name
template<>
template<>
void testObj::test<18>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.referenceurl.name"), Rule::Mode::EQUALS, "some name", fmt_ );
  ensure("invalid url's name", r.compute(leaf_)==true );
}

// test url.url
template<>
template<>
void testObj::test<19>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.referenceurl.url"), Rule::Mode::EQUALS, "http://gnu.org", fmt_ );
  ensure("invalid url's adderss", r.compute(leaf_)==true );
}

// test host.name
template<>
template<>
void testObj::test<20>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.$.name"), Rule::Mode::EQUALS, "kszy.net", fmt_ );
  ensure("invalid hosts's dns name", r.compute(leaf_)==true );
}

// test service.name
template<>
template<>
void testObj::test<21>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.services.*.name"), Rule::Mode::EQUALS, "service name", fmt_ );
  ensure("invalid service's name", r.compute(leaf_)==true );
}

// test service.port
template<>
template<>
void testObj::test<22>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.services.*.port"), Rule::Mode::EQUALS, "42", fmt_ );
  ensure("invalid service's port", r.compute(leaf_)==true );
}

// test service.protocol
template<>
template<>
void testObj::test<23>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.services.*.protocol"), Rule::Mode::EQUALS, "magic_proto", fmt_ );
  ensure("invalid service's protocol", r.compute(leaf_)==true );
}

// test service.url.name
template<>
template<>
void testObj::test<24>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.services.*.referenceurl.name"), Rule::Mode::EQUALS, "some name", fmt_ );
  ensure("invalid service's url name", r.compute(leaf_)==true );
}

// test process.url
template<>
template<>
void testObj::test<25>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.referenceurl.name"), Rule::Mode::EQUALS, "some name", fmt_ );
  ensure("invalid process' url name", r.compute(leaf_)==true );
}

// test process.path
template<>
template<>
void testObj::test<26>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.path"), Rule::Mode::EQUALS, "/path/to/bin", fmt_ );
  ensure("invalid process' path", r.compute(leaf_)==true );
}

// test process.name
template<>
template<>
void testObj::test<27>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.name"), Rule::Mode::EQUALS, "name", fmt_ );
  ensure("invalid process' name", r.compute(leaf_)==true );
}

// test process.md5
template<>
template<>
void testObj::test<28>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.md5"),
                Rule::Mode::EQUALS,
                "01234567890123456789012345678901",
                fmt_ );
  ensure("invalid process' md5", r.compute(leaf_)==true );
}

// test process.pid
template<>
template<>
void testObj::test<29>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.pid"), Rule::Mode::EQUALS, "666", fmt_ );
  ensure("invalid process' pid", r.compute(leaf_)==true );
}

// test process.uid
template<>
template<>
void testObj::test<30>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.uid"), Rule::Mode::EQUALS, "42", fmt_ );
  ensure("invalid process' uid", r.compute(leaf_)==true );
}

// test process.username
template<>
template<>
void testObj::test<31>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.username"), Rule::Mode::EQUALS, "looser", fmt_ );
  ensure("invalid process' username", r.compute(leaf_)==true );
}

// test process.paramters
template<>
template<>
void testObj::test<32>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.processes.*.parameters"), Rule::Mode::EQUALS, "-a -b -c", fmt_ );
  ensure("invalid process' arguments", r.compute(leaf_)==true );
}

// test for-each mark - negative test
template<>
template<>
void testObj::test<33>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.*.ip"), Rule::Mode::EQUALS, "1.1.1.1", fmt_ );
  ensure("for-each didn't failed on different entries", r.compute(leaf_)==false );
}

// test for-each mark - positive test
template<>
template<>
void testObj::test<34>(void)
{
  const Rule r( Rule::Path("metaalert.alert.targets.*.ip"), Rule::Mode::EQUALS, "3.2.2.3", fmt_ );
  ensure("for-each failed for the same entries", r.compute(leaf_)==true );
}

// test for-any mark - negative test
template<>
template<>
void testObj::test<35>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.$.ip"), Rule::Mode::EQUALS, "9.1.1.1", fmt_ );
  ensure("for-any didn't failed on non-exisitng entry", r.compute(leaf_)==false );
}

// test for-any mark - positive test
template<>
template<>
void testObj::test<36>(void)
{
  const Rule r( Rule::Path("metaalert.alert.sources.$.ip"), Rule::Mode::EQUALS, "2.2.2.2", fmt_ );
  ensure("for-any failed on existing entry", r.compute(leaf_)==true );
}

// test Mode::REGEXP - positive case
template<>
template<>
void testObj::test<37>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::REGEXP, "^a.*e$", fmt_ );
  ensure("equals failed", r.compute(leaf_)==true );
}

// test Mode::REGEXP - negative case
template<>
template<>
void testObj::test<38>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::REGEXP, "^A.*E$", fmt_ );
  ensure("equals failed", r.compute(leaf_)==false );
}

// test Mode::REGEXPCI - positive case
template<>
template<>
void testObj::test<39>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::REGEXPCI, "^A.*E$", fmt_ );
  ensure("equals failed", r.compute(leaf_)==true );
}

// test Mode::REGEXPCI - negative case
template<>
template<>
void testObj::test<40>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::REGEXPCI, "^x.*E$", fmt_ );
  ensure("equals failed", r.compute(leaf_)==false );
}

// test Mode::LESSTHAN - positive case
template<>
template<>
void testObj::test<41>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::LESSTHAN, "xxx", fmt_ );
  ensure("equals failed", r.compute(leaf_)==true );
}

// test Mode::LESSTHAN - negative case
template<>
template<>
void testObj::test<42>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::LESSTHAN, "aaa", fmt_ );
  ensure("equals failed", r.compute(leaf_)==false );
}

// test Mode::GREATERTHAN - positive case
template<>
template<>
void testObj::test<43>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::GREATERTHAN, "aa", fmt_ );
  ensure("equals failed", r.compute(leaf_)==true );
}

// test Mode::GREATERTHAN - negative case
template<>
template<>
void testObj::test<44>(void)
{
  const Rule r( Rule::Path("metaalert.alert.name"), Rule::Mode::GREATERTHAN, "xxx", fmt_ );
  ensure("equals failed", r.compute(leaf_)==false );
}

} // namespace tut
