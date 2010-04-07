/*
 * ParseInputs.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParseInputs.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

struct TestClass
{
  // return copyied, parsed subtree
  Node getInputs(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parseContent(xml);
    return tree.getRoot().getChild("inputs");
  }

  // return copyied persistency config
  ParseInputs getConf(const char *xml="testdata/sample_config.xml") const
  {
    assert(xml!=NULL);
    FileReader             fr(xml);
    const Node             n=getInputs( fr.getString() );
    const ParseInputs pp(n);
    return pp;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParseInputs");
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

// test if all inputs are there
template<>
template<>
void testObj::test<2>(void)
{
  const ParseInputs             pp  =getConf();
  const InputsConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
}

// check input with no options
template<>
template<>
void testObj::test<3>(void)
{
  const ParseInputs             pp  =getConf();
  const InputsConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg[0].getType(), "noopts");
  ensure_equals("invalid number of options", cfg[0].getOptions().size(), 0u);
}

// check input with options
template<>
template<>
void testObj::test<4>(void)
{
  const ParseInputs             pp  =getConf();
  const InputsConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg[1].getType(), "something");
  ensure_equals("invalid number of options", cfg[1].getOptions().size(), 2u);
  ensure_equals("invalid option's 1 value", cfg[1]["opt6"], "or");
  ensure_equals("invalid option's 2 value", cfg[1]["opt9"], "XOR");
}

} // namespace tut
