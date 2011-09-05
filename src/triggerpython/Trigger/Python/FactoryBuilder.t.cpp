/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Python/FactoryBuilder.hpp"

using namespace std;
using namespace Trigger::Python;

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
    opts_["name"]="the interpreted one";

    opts_["script"]="testdata/do_nothing.py";

    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Python/FactoryBuilder");
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

// test throw missing script entry
template<>
template<>
void testObj::test<10>(void)
{
  setValid();
  opts_.erase("script");
  ensureThrow(opts_);
}

} // namespace tut
