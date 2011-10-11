/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/FactoryBuilder.hpp"

using namespace std;
using namespace Trigger::SnortSam;

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
      tut::fail("build() didn't throw on missing/invalid paramter");
    }
    catch(const std::runtime_error&)
    {
      // this is expected
    }
  }

  void setValid(void)
  {
    opts_["name"]    ="triggername";
    opts_["host"]    ="snort.host";
    opts_["port"]    ="6699";
    opts_["key"]     ="p@$$w0rD";
    opts_["who"]     ="src dst";
    opts_["how"]     ="in out";
    opts_["duration"]="42";

    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// test creation with valid config
template<>
template<>
void testObj::test<1>(void)
{
  setValid();
  fb_.build(opts_); // must not throw
}

// test creation with valid config, w-out thresholds
template<>
template<>
void testObj::test<2>(void)
{
  setValid();
  opts_.erase("severity_threshold");
  opts_.erase("alerts_count_threshold");
  fb_.build(opts_); // must not throw
}

// test exception when host is not set
template<>
template<>
void testObj::test<3>(void)
{
  setValid();
  opts_.erase("host");
  ensureThrow(opts_);
}

// test exception when port is not set
template<>
template<>
void testObj::test<4>(void)
{
  setValid();
  opts_.erase("port");
  ensureThrow(opts_);
}

// test exception when port is invalid
template<>
template<>
void testObj::test<5>(void)
{
  setValid();
  opts_["port"]="-11";
  ensureThrow(opts_);
}

// key is not set
template<>
template<>
void testObj::test<6>(void)
{
  setValid();
  opts_.erase("key");
  ensureThrow(opts_);
}

// test when 'who' is not set
template<>
template<>
void testObj::test<7>(void)
{
  setValid();
  opts_.erase("who");
  ensureThrow(opts_);
}

// test when 'who' has invalid value
template<>
template<>
void testObj::test<8>(void)
{
  setValid();
  opts_["who"]="src aaaa";
  ensureThrow(opts_);
}

// test when 'how' is missing
template<>
template<>
void testObj::test<9>(void)
{
  setValid();
  opts_.erase("how");
  ensureThrow(opts_);
}

// test when 'how' has invalid value
template<>
template<>
void testObj::test<10>(void)
{
  setValid();
  opts_["how"]="in blah";
  ensureThrow(opts_);
}

// test throw on missing 'duration'
template<>
template<>
void testObj::test<11>(void)
{
  setValid();
  opts_.erase("duration");
  ensureThrow(opts_);
}

// test when duration is non-positive
template<>
template<>
void testObj::test<12>(void)
{
  setValid();
  opts_["duration"]="-10";
  ensureThrow(opts_);
}

} // namespace tut
