/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <boost/filesystem.hpp>

#include "Trigger/Whois/FactoryBuilder.hpp"
#include "Commons/Filesystem/createFile.hpp"

using namespace std;
using namespace Trigger::Whois;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  typedef FactoryBuilder::Options Options;

  void ensureThrow(const Options &opts) const
  {
    bool thrown=false;
    try
    {
      fb_.build(opts);
      thrown=true;
      tut::fail("build() didn't throw on missing/invalid paramter");
    }
    catch(const std::exception&)
    {
      if(thrown)
        throw;
      // this is expected
    }
  }

  void setValid(void)
  {
    opts_["name"]                  ="somename";
    opts_["template"]              ="testdata/message.tmpl";
    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
  }

  boost::filesystem::path p_;
  FactoryBuilder          fb_;
  Options                 opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Whois/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "whois");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  setValid();
  FactoryBuilder::FactoryPtr ptr=fb_.build(opts_);
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test throw on missing path
template<>
template<>
void testObj::test<3>(void)
{
  setValid();
  opts_.erase("template");
  ensureThrow(opts_);
}

// check throw when path is too short
template<>
template<>
void testObj::test<4>(void)
{
  setValid();
  opts_["template"]="/i/do/not/exist.tmpl";    // this cannot be valid path
  ensureThrow(opts_);
}

// set no-throw when severity_threshold is not set
template<>
template<>
void testObj::test<5>(void)
{
  setValid();
  opts_.erase("severity_threshold");
  fb_.build(opts_);     // must not throw
}

// check throw when severity_threshold is invalid
template<>
template<>
void testObj::test<6>(void)
{
  setValid();
  opts_["severity_threshold"]="narf?";
  ensureThrow(opts_);
}

// test no-thrown when alerts_count_threshold is not set
template<>
template<>
void testObj::test<7>(void)
{
  setValid();
  opts_.erase("alerts_count_threshold");
  fb_.build(opts_);     // must not throw
}

// test throw when alerts_count_threshold is invalid
template<>
template<>
void testObj::test<8>(void)
{
  setValid();
  opts_["alerts_count_threshold"]="damage.inc";
  ensureThrow(opts_);
}

// test no throw when both alerts_count_threshold and severity_threshold are not set
template<>
template<>
void testObj::test<9>(void)
{
  setValid();
  opts_.erase("severity_threshold");
  opts_.erase("alerts_count_threshold");
  fb_.build(opts_);     // must not throw
}

} // namespace tut
