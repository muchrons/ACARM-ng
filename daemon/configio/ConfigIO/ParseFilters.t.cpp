/*
 * ParseFilters.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParseFilters.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

struct TestClass
{
  // return copyied, parsed subtree
  Node getFilters(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parseContent(xml);
    return tree.getRoot().getChild("filters");
  }

  // return copyied persistency config
  ParseFilters getConf(const char *xml="testdata/sample_config.xml") const
  {
    assert(xml!=NULL);
    FileReader             fr(xml);
    const Node             n=getFilters( fr.getString() );
    const ParseFilters pp(n);
    return pp;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParseFilters");
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

// test if all filters are there
template<>
template<>
void testObj::test<2>(void)
{
  const ParseFilters             pp  =getConf();
  const FiltersConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
}

// check filter with no options
template<>
template<>
void testObj::test<3>(void)
{
  const ParseFilters             pp  =getConf();
  const FiltersConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg[0].getType(), "noopts");
  ensure_equals("invalid number of options", cfg[0].getOptions().size(), 0u);
}

// check filter with options
template<>
template<>
void testObj::test<4>(void)
{
  const ParseFilters             pp  =getConf();
  const FiltersConfigCollection &cfg =pp.getConfig();
  ensure_equals("invalid number of entries", cfg.size(), 2u);
  ensure_equals("invalid type", cfg[1].getType(), "something");
  ensure_equals("invalid number of options", cfg[1].getOptions().size(), 2u);
  ensure_equals("invalid option's 1 value", cfg[1]["opt1"], "narf");
  ensure_equals("invalid option's 2 value", cfg[1]["opt2"], "abc");
}

} // namespace tut
