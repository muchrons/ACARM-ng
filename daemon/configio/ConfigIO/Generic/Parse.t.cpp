/*
 * Parse.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/FileReader.hpp"
#include "ConfigIO/Generic/Config.hpp"
#include "ConfigIO/Generic/Parse.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO::Generic;

namespace
{

struct TestConfig: public Config<TestConfig>
{
  TestConfig(const TypeName &type,
             const Options  &options):
    Config<TestConfig>(type, options)
  {
  }
};

typedef std::vector<TestConfig>                 TestConfigCollection;
typedef Parse<TestConfig, TestConfigCollection> TestParse;

struct TestClass
{
  // return copyied, parsed subtree
  Node get(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parseContent(xml);
    return tree.getRoot().getChild("whatever");
  }

  // return copyied persistency config
  TestParse getConf(const char *xml="testdata/generic_parse_data.xml") const
  {
    assert(xml!=NULL);
    ConfigIO::FileReader fr(xml);
    const Node           n=get( fr.getString() );
    const TestParse      pp(n);
    return pp;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Generic/Parse");
} // unnamed namespace


namespace tut
{

// smke test for reading valid configuration
template<>
template<>
void testObj::test<1>(void)
{
  getConf();
}

// test if all config sets are there
template<>
template<>
void testObj::test<2>(void)
{
  const TestParse             pp =getConf();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2);
}

// check set with no options
template<>
template<>
void testObj::test<3>(void)
{
  const TestParse             pp =getConf();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2);
  ensure_equals("invalid type", cfg.at(0).getType(), "noopts");
  ensure_equals("invalid number of options", cfg.at(0).getOptions().size(), 0);
}

// check set with options
template<>
template<>
void testObj::test<4>(void)
{
  const TestParse             pp =getConf();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2);
  ensure_equals("invalid type", cfg.at(1).getType(), "something");
  ensure_equals("invalid number of options", cfg.at(1).getOptions().size(), 2);
  ensure_equals("invalid option's 1 value", cfg.at(1)["opt4"], "alice");
  ensure_equals("invalid option's 2 value", cfg.at(1)["opt7"], "cat");
}

} // namespace tut
