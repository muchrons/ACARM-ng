/*
 * Analyzer.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <memory>

#include "Persistency/Analyzer.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
  TestClass(void):
    ver_("v1.2.3"),
    os_("Linux 2.6.66"),
    ip_( Analyzer::IPv4::from_string("1.2.3.4") )
  {
  }

  const Analyzer::Version         ver_;
  const Analyzer::OperatingSystem os_;
  const Analyzer::IP              ip_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Analyzer");
} // unnamed namespace


namespace tut
{

// test creating regular analyzer
template<>
template<>
void testObj::test<1>(void)
{
  const AnalyzerPtrNN a( new Analyzer("analyzer1", ver_, os_, &ip_) );

  ensure_equals("invalid name", a->getName().get(), string("analyzer1") );

  ensure("bat version pointer", a->getVersion()==ver_);
  ensure_equals("invalid version", a->getVersion().get(), string("v1.2.3") );

  ensure("bat OS pointer", a->getOperatingSystem()==os_);
  ensure_equals("invalid OS name", a->getOperatingSystem().get(), string("Linux 2.6.66") );

  ensure("NULL IP", a->getIP()!=NULL );
  ensure("bat IP pointer", a->getIP()!=&ip_);
  ensure("invalid IP", *a->getIP()==ip_);
}

// test creating with NULL name
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    const Analyzer a(NULL, ver_, os_, &ip_);
    fail("analyzer didn't throw on NULL name");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test creating with NULL version
template<>
template<>
void testObj::test<3>(void)
{
  const Analyzer a("analyzer1", NULL, os_, &ip_);
  ensure("version not NULL", a.getVersion()==NULL );
}

// test creating with NULL OS
template<>
template<>
void testObj::test<4>(void)
{
  const Analyzer a("analyzer1", ver_, NULL, &ip_);
  ensure("OS not NULL", a.getOperatingSystem()==NULL );
}

// test creating with NULL IP
template<>
template<>
void testObj::test<5>(void)
{
  const Analyzer a("analyzer1", ver_, os_, NULL);
  ensure("IP not NULL", a.getIP()==NULL );
}

// test comparing for different name
template<>
template<>
void testObj::test<6>(void)
{
  const Analyzer::Version         ver=ver_;
  const Analyzer::OperatingSystem os =os_;
  const Analyzer::IP              ip =ip_;

  const Analyzer a1a("analyzer1", ver_, os_, &ip_);
  const Analyzer a1b("analyzer1", ver,  os,  &ip );

  const Analyzer::Version         ver2=ver_;
  const Analyzer::OperatingSystem os2 =os_;
  const Analyzer::IP              ip2 =ip_;
  const Analyzer                  a2("analyzer2", ver2, os2, &ip2);
  TestHelpers::checkEquality(a1a, a1b, a2);
}

// test comparing for different versions
template<>
template<>
void testObj::test<7>(void)
{
  const Analyzer                  a1("analyzer1", ver_, os_, &ip_);
  const Analyzer::Version         ver2("v4.2");
  const Analyzer::OperatingSystem os2 =os_;
  const Analyzer::IP              ip2 =ip_;
  const Analyzer                  a2("analyzer1", ver2, os2, &ip2);
  TestHelpers::checkEquality(a1, a2);
}

// test comparing for different OS'
template<>
template<>
void testObj::test<8>(void)
{
  const Analyzer                  a1("analyzer1", ver_, os_, &ip_);
  const Analyzer::Version         ver2=ver_;
  const Analyzer::OperatingSystem os2("OS2/W");
  const Analyzer::IP              ip2 =ip_;
  const Analyzer                  a2("analyzer1", ver2, os2, &ip2);
  TestHelpers::checkEquality(a1, a2);
}

// test comparing for different IPs
template<>
template<>
void testObj::test<9>(void)
{
  const Analyzer a1("analyzer1", ver_, os_, &ip_);

  const Analyzer::Version         ver2=ver_;
  const Analyzer::OperatingSystem os2 =os_;
  const Analyzer::IP              ip2( Analyzer::IPv4::from_string("4.3.2.1") );
  const Analyzer                  a2("analyzer1", ver2, os2, &ip2);
  TestHelpers::checkEquality(a1, a2);
}

// test if size is reasonable
template<>
template<>
void testObj::test<10>(void)
{
  ensure_equals("size of output object is not reasonable big", sizeof(Analyzer::ID), 8);
}

} // namespace tut
