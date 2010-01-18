/*
 * ParsePersistency.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParsePersistency.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

const char *xmlOK1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  "    <user>login</user>"
  "    <pass>s3cr3t</pass>"
  "    <host>my.domain.pl</host>"
  "    <port>666</port>"
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  ""                            // <-- no 'user' field
  "    <pass>s3cr3t</pass>"
  "    <host>my.domain.pl</host>"
  "    <port>666</port>"
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr2=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  "    <user>login</user>"
  ""                            // <-- no 'pass' filed
  "    <host>my.domain.pl</host>"
  "    <port>666</port>"
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr3=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  "    <user>login</user>"
  "    <pass>s3cr3t</pass>"
  ""                            // <-- no 'host' field
  "    <port>666</port>"
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr4=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  "    <user>login</user>"
  "    <pass>s3cr3t</pass>"
  "    <host>my.domain.pl</host>"
  ""                            // <-- no 'port' field
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr5=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  ""                            // <-- no 'type' field
  "    <user>login</user>"
  "    <pass>s3cr3t</pass>"
  "    <host>my.domain.pl</host>"
  "    <port>666</port>"
  "    <dbname>mydatabase</dbname>"
  "  </persistency>"
  "</acarm_ng>"
  "";

const char *xmlErr6=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <persistency>"
  "    <type>PostgreSQL</type>"
  "    <user>login</user>"
  "    <pass>s3cr3t</pass>"
  "    <host>my.domain.pl</host>"
  "    <port>666</port>"
  ""                            // <-- no 'dbname' field
  "  </persistency>"
  "</acarm_ng>"
  "";


struct ParsePersistencyTestClass
{
  // return copyied, parsed subtree
  Node getPersistency(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parse(xml);
    return tree.getRoot().getChild("persistency");
  }

  // return copyied persistency config
  ParsePersistency getConf(const char *xml) const
  {
    assert(xml!=NULL);
    const Node             n=getPersistency(xml);
    const ParsePersistency pp(n);
    return pp;
  }

  // check if given xml raises exception
  void ensureFails(const char *xml) const
  {
    assert(xml!=NULL);
    try
    {
      getConf(xml);
      tut::fail("parsing didn't throw on missing field");
    }
    catch(const XML::Exception&)
    {
      // this is expected
    }
  }
};

typedef ParsePersistencyTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParsePersistency");
} // unnamed namespace


namespace tut
{

// selft test
template<>
template<>
void testObj::test<1>(void)
{
  const Node n=getPersistency(xmlOK1);
  ensure_equals("invalid node returned", n.getName(), "persistency");
}

// test if parsing does not throw
template<>
template<>
void testObj::test<2>(void)
{
  getConf(xmlOK1);
}

// test if parameters were read correctly
template<>
template<>
void testObj::test<3>(void)
{
  const ParsePersistency  pp=getConf(xmlOK1);
  const PersistencyConfig pc=pp.getConfig();
  ensure_equals("invalid user", pc.getUser(),       "login"       );
  ensure_equals("invalid pass", pc.getPassword(),   "s3cr3t"      );
  ensure_equals("invalid host", pc.getHost(),       "my.domain.pl");
  ensure_equals("invalid port", pc.getPortNumber(), 666           );
}

// test missing 'user' field
template<>
template<>
void testObj::test<4>(void)
{
  ensureFails(xmlErr1);
}

// test missing 'pass' field
template<>
template<>
void testObj::test<5>(void)
{
  ensureFails(xmlErr2);
}

// test missing 'host' field
template<>
template<>
void testObj::test<6>(void)
{
  ensureFails(xmlErr3);
}

// test missing 'port' field
template<>
template<>
void testObj::test<7>(void)
{
  ensureFails(xmlErr4);
}

// test missing 'type' field
template<>
template<>
void testObj::test<8>(void)
{
  ensureFails(xmlErr5);
}

// test missing 'dbname' field
template<>
template<>
void testObj::test<9>(void)
{
  ensureFails(xmlErr6);
}

} // namespace tut
