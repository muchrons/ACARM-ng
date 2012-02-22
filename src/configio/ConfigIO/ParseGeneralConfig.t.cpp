/*
 * ParseGeneralConfig.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParseGeneralConfig.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

struct TestClass
{
  // return copyied, parsed subtree
  Node getNodes(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parseContent(xml);
    return tree.getRoot().getChild("general");
  }

  // return copyied appenders configs
  ParseGeneralConfig getConfig(const char *xml) const
  {
    const Node               n=getNodes(xml);
    const ParseGeneralConfig pgc(n);
    return pgc;
  }

  void testThrow(const char *xml) const
  {
    try
    {
      ParseGeneralConfig pgc( getConfig(xml) );
      tut::fail("c-tor didn't throw on invalid config");
    }
    catch(const XML::Exception &)
    {
      // this is expected...
    }
    catch(const ConfigIO::Exception &)
    {
      // this is expected...
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParseGeneralConfig");
} // unnamed namespace


namespace tut
{

// test for parsing valid config
template<>
template<>
void testObj::test<1>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  const ParseGeneralConfig pgc( getConfig(xml) );
  const GeneralConfig      gc=pgc.getConfig();
  ensure_equals("invalid url", gc.getWUIUrl(), "http://go.to.hell");
  ensure_equals("invalid cleanup interval", gc.getCleanupInterval(), 42u);
  ensure_equals("invalid cleanup older", gc.getCleanupOlder(), 997u);
  ensure_equals("invalid plugins dir", gc.getPluginsDir(), "/some/dir/name");
}

// test exception when URL is missing
template<>
template<>
void testObj::test<2>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
//    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test exception when cleanup interval is missing
template<>
template<>
void testObj::test<3>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
//    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test throw on invalid URL
template<>
template<>
void testObj::test<4>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>XYZ://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test throw on invalid cleanup interval
template<>
template<>
void testObj::test<5>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>-42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test exception when cleanup older is missing
template<>
template<>
void testObj::test<6>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
//    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test exception when plugins dir is missing
template<>
template<>
void testObj::test<7>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
//    "    <pluginsDir>/some/dir/name</pluginsDir>"
    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}

// test exception when memory limit is missing
template<>
template<>
void testObj::test<8>(void)
{
  const char *xml=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<acarm_ng>"
    "  <general>"
    "    <url>http://go.to.hell</url>"
    "    <cleanupInterval>42</cleanupInterval>"
    "    <cleanupOlder>997</cleanupOlder>"
    "    <pluginsDir>/some/dir/name</pluginsDir>"
//    "    <memoryLimit>1983</memoryLimit>"
    "  </general>"
    "</acarm_ng>"
    "";
  testThrow(xml);
}


} // namespace tut
