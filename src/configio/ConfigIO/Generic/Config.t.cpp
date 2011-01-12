/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Generic/Config.hpp"

using namespace std;
using namespace ConfigIO::Generic;

namespace
{

struct Cfg: public Config<Cfg>
{
  Cfg(void):
    Config<Cfg>( "type1", makeOptions() )
  {
  }

  Options makeOptions(void)
  {
    Options opts;
    opts["abc"] ="def";
    opts["narf"]="fran";
    return opts;
  }
};

struct TestClass
{
  Cfg cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Generic/Config");
} // unnamed namespace


namespace tut
{

// test type name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", cfg_.getType(), "type1");
}

// test opions getting
template<>
template<>
void testObj::test<2>(void)
{
  const Cfg::Options &opts=cfg_.getOptions();
  ensure("empty collection", opts.begin()!=opts.end() );
}

// test [] operator access to elements
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid value for element 1", cfg_["abc"],  "def" );
  ensure_equals("invalid value for element 2", cfg_["narf"], "fran");
}

// test if [] operator throws on unknown element
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    cfg_["non-exisiting-paramter"];
    fail("operator[] did'nt throw for non-existing entry");
  }
  catch(const ConfigIO::ExceptionNoSuchParameter &)
  {
    // this is expected
  }
}

// test get() for existing value
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid value for 'abc'", cfg_.get("abc"), std::string("def") );
}

// test get() for non-exisitng value
template<>
template<>
void testObj::test<6>(void)
{
  ensure("non-NULL value returned for nonexisitng element", cfg_.get("idontexist")==NULL );
}

} // namespace tut
