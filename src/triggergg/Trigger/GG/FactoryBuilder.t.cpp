/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Trigger/GG/FactoryBuilder.hpp"

using namespace std;
using namespace Trigger::GG;

namespace
{

struct TestClass
{
  typedef FactoryBuilder::Options Options;

  void ensureThrow(const Options &opts) const
  {
    try
    {
      fb_.build(opts);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const Commons::Convert::ExceptionConversionError&)
    {
      // this is expected
    }
    catch(const ConfigIO::ExceptionNoSuchParameter&)
    {
      // this is expected
    }
    catch(const Trigger::ExceptionInvalidValue&)
    {
      // this is expected
    }
  }

  void setValid(void)
  {
    opts_["name"]                  ="somename";
    opts_["uin"]                   ="123";
    opts_["password"]              ="secr3t";
    opts_["receivers"]             ="69";
    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "gg");
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

// test throw on missing UID
template<>
template<>
void testObj::test<3>(void)
{
  setValid();
  opts_.erase("uin");
  ensureThrow(opts_);
}

// test throw on invalid UID
template<>
template<>
void testObj::test<4>(void)
{
  setValid();
  opts_["uin"]="kszy";
  ensureThrow(opts_);
}

// test thrown on no password
template<>
template<>
void testObj::test<5>(void)
{
  setValid();
  opts_.erase("password");
  ensureThrow(opts_);
}

// test throw on no receiver's UID
template<>
template<>
void testObj::test<6>(void)
{
  setValid();
  opts_.erase("receivers");
  ensureThrow(opts_);
}

// test throw on invalid receiver's UID
template<>
template<>
void testObj::test<7>(void)
{
  setValid();
  opts_["receivers"]="-42";
  ensureThrow(opts_);
}

// set no-throw when severity_threshold is not set
template<>
template<>
void testObj::test<8>(void)
{
  setValid();
  opts_.erase("severity_threshold");
  fb_.build(opts_);     // must not throw
}

// check throw when severity_threshold is invalid
template<>
template<>
void testObj::test<9>(void)
{
  setValid();
  opts_["severity_threshold"]="narf?";
  ensureThrow(opts_);
}

// test no-thrown when alerts_count_threshold is not set
template<>
template<>
void testObj::test<10>(void)
{
  setValid();
  opts_.erase("alerts_count_threshold");
  fb_.build(opts_);     // must not throw
}

// test throw when alerts_count_threshold is invalid
template<>
template<>
void testObj::test<11>(void)
{
  setValid();
  opts_["alerts_count_threshold"]="damage.inc";
  ensureThrow(opts_);
}

// test no throw when both alerts_count_threshold and severity_threshold are not set
template<>
template<>
void testObj::test<12>(void)
{
  setValid();
  opts_.erase("severity_threshold");
  opts_.erase("alerts_count_threshold");
  fb_.build(opts_);     // must not throw
}

// test multiple receivers
template<>
template<>
void testObj::test<13>(void)
{
  setValid();
  opts_["receivers"]="1234 5678";
  fb_.build(opts_);     // must not throw
}

} // namespace tut
