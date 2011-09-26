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

  TestConfig(const TypeName                       &type,
             const Options                        &options,
             const ConfigIO::Preprocessor::Config &ppCfg):
    Config<TestConfig>(type, options, ppCfg)
  {
  }
};

typedef std::vector<TestConfig>                        TestConfigCollection;
typedef Parse<TestConfig, TestConfigCollection, false> TestParse;
typedef Parse<TestConfig, TestConfigCollection, true>  TestParsePP;

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

  // return copyied persistency config - named
  template<typename TTestParse>
  TTestParse getConf(const char *xml="testdata/generic_named_parse_data.xml") const
  {
    assert(xml!=NULL);
    ConfigIO::FileReader fr(xml);
    const Node           n=get( fr.getString() );
    const TTestParse     pp(n);
    return pp;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Generic/Parse");
} // unnamed namespace


namespace tut
{

// smoke test for reading valid "named" configuration
template<>
template<>
void testObj::test<1>(void)
{
  getConf<TestParse>();
}

// test if all config sets are there - "named"
template<>
template<>
void testObj::test<2>(void)
{
  const TestParse             pp =getConf<TestParse>();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
}

// check set with no options - "named"
template<>
template<>
void testObj::test<3>(void)
{
  const TestParse             pp =getConf<TestParse>();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(0).getType(), "noopts");
  ensure_equals("invalid number of options", cfg.at(0).getOptions().size(), 1u);
  ensure_equals("invalid option's 1 value", cfg.at(0)["name"], "noopts");
}

// check set with options - "named"
template<>
template<>
void testObj::test<4>(void)
{
  const TestParse             pp =getConf<TestParse>();
  const TestConfigCollection &cfg=pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg.at(1).getType(), "something");
  ensure_equals("invalid number of options", cfg.at(1).getOptions().size(), 3u);
  ensure_equals("invalid option's 1 value", cfg.at(1)["opt4"], "alice");
  ensure_equals("invalid option's 2 value", cfg.at(1)["opt7"], "cat");
  ensure_equals("invalid option's 3 value", cfg.at(1)["name"], "other name");
}

// check sample configuration w-out preprocessor
template<>
template<>
void testObj::test<5>(void)
{
  const TestParsePP           pp =getConf<TestParsePP>("testdata/generic_named_parse_data_no_preproc.xml");
  const TestConfigCollection &cfg=pp.getConfig();
  ensure("preprocessor is set", cfg.at(0).getPreprocessorConfig()==NULL);
}

// check sample configuration with preprocessor
template<>
template<>
void testObj::test<6>(void)
{
  const TestParsePP           pp =getConf<TestParsePP>("testdata/generic_named_parse_data_with_preproc.xml");
  const TestConfigCollection &cfg=pp.getConfig();
  ensure("preprocessor is not set", cfg.at(0).getPreprocessorConfig()!=NULL);
  ensure_equals("invalid number of sections", cfg.at(0).getPreprocessorConfig()->getSections().size(), 1u);
}

} // namespace tut
