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
typedef Parse<TestConfig, TestConfigCollection, true> TestParseNamed;

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

  // return copyied persistency config - named
  TestParseNamed getConfNamed(const char *xml="testdata/generic_named_parse_data.xml") const
  {
    assert(xml!=NULL);
    ConfigIO::FileReader  fr(xml);
    const Node            n=get( fr.getString() );
    const TestParseNamed  pp(n);
    return pp;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Generic/Parse");
} // unnamed namespace


namespace tut
{

// smoke test for reading valid configuration
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
  ensure_equals("invalid number of entries", cfg.size(), 2u);
}

// check set with no options
template<>
template<>
void testObj::test<3>(void)
{
  const TestParse             pp =getConf();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(0).getType(), "noopts");
  ensure_equals("invalid number of options", cfg.at(0).getOptions().size(), 0u);
}

// check set with options
template<>
template<>
void testObj::test<4>(void)
{
  const TestParse             pp =getConf();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(1).getType(), "something");
  ensure_equals("invalid number of options", cfg.at(1).getOptions().size(), 2u);
  ensure_equals("invalid option's 1 value", cfg.at(1)["opt4"], "alice");
  ensure_equals("invalid option's 2 value", cfg.at(1)["opt7"], "cat");
}

// smoke test for reading valid "named" configuration
template<>
template<>
void testObj::test<5>(void)
{
  getConfNamed();
}

// test if all config sets are there - "named"
template<>
template<>
void testObj::test<6>(void)
{
  const TestParseNamed       pp =getConfNamed();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
}

// check set with no options - "named"
template<>
template<>
void testObj::test<7>(void)
{
  const TestParseNamed       pp =getConfNamed();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(0).getType(), "noopts");
  ensure_equals("invalid number of options", cfg.at(0).getOptions().size(), 1u);
  ensure_equals("invalid option's 1 value", cfg.at(0)["name"], "noopts");
}

// check set with options - "named"
template<>
template<>
void testObj::test<8>(void)
{
  const TestParseNamed       pp =getConfNamed();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(1).getType(), "something");
  ensure_equals("invalid number of options", cfg.at(1).getOptions().size(), 3u);
  ensure_equals("invalid option's 1 value", cfg.at(1)["opt4"], "alice");
  ensure_equals("invalid option's 2 value", cfg.at(1)["opt7"], "cat");
  ensure_equals("invalid option's 3 value", cfg.at(1)["name"], "other name");
}

} // namespace tut
