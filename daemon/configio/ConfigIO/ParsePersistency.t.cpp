/*
 * ParsePersistency.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParsePersistency.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

struct ParsePersistencyTestClass
{
  // return copyied, parsed subtree
  Node getPersistency(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parseContent(xml);
    return tree.getRoot().getChild("persistency");
  }

  // return copyied persistency config
  ParsePersistency getConf(const char *xml="testdata/sample_config.xml") const
  {
    assert(xml!=NULL);
    FileReader             fr(xml);
    const Node             n=getPersistency( fr.getString() );
    const ParsePersistency pp(n);
    return pp;
  }
};

typedef ParsePersistencyTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParsePersistency");
} // unnamed namespace


namespace tut
{

// test name
template<>
template<>
void testObj::test<1>(void)
{
  const ParsePersistency pp=getConf();
  ensure_equals("invalid node returned", pp.getConfig().getType(), "postgres");
}

// test if all options are there
template<>
template<>
void testObj::test<2>(void)
{
  const ParsePersistency      pp  =getConf();
  const PersistencyConfig    &cfg =pp.getConfig();
  PersistencyConfig::Options  opts=cfg.getOptions();
  ensure_equals("invalid host",   opts["host"],   "db.server.my.domain.org");
  ensure_equals("invalid port",   opts["port"],   "5432");
  ensure_equals("invalid dbname", opts["dbname"], "kszy");
  ensure_equals("invalid user",   opts["user"],   "john");
  ensure_equals("invalid pass",   opts["pass"],   "$3cr3t");
}

// test if no extra paremters (like 'type') are present
template<>
template<>
void testObj::test<3>(void)
{
  const ParsePersistency      pp  =getConf();
  const PersistencyConfig    &cfg =pp.getConfig();
  PersistencyConfig::Options  opts=cfg.getOptions();
  ensure_equals("invalid element's count", opts.size(), 5u);
}

} // namespace tut
