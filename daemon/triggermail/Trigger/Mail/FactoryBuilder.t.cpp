/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/FactoryBuilder.hpp"

using namespace std;
using namespace Trigger::Mail;

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
    opts_["from"]    ="from@address";
    opts_["to"]      ="to@address";
    opts_["server"]  ="server.address";
    opts_["port"]    ="69";
    opts_["tls"]     ="false";

    opts_["user"]    ="narf";
    opts_["password"]="fran";

    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/FactoryBuilder");
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

// test creating valid config, w-out authorization
template<>
template<>
void testObj::test<3>(void)
{
  setValid();
  opts_.erase("user");
  opts_.erase("password");
  fb_.build(opts_); // must not throw
}

// test for error when password is not set, but authorization is required
template<>
template<>
void testObj::test<4>(void)
{
  setValid();
  opts_.erase("password");
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

// test when TLS is 'true'
template<>
template<>
void testObj::test<6>(void)
{
  setValid();
  opts_["tls"]="true";
  fb_.build(opts_);     // must not throw
}

// test TLS 'false'
template<>
template<>
void testObj::test<7>(void)
{
  setValid();
  opts_["tls"]="false";
  fb_.build(opts_);     // must not throw
}

// test throw on invalid TLS value
template<>
template<>
void testObj::test<8>(void)
{
  setValid();
  opts_["tls"]="faLSE";
  ensureThrow(opts_);
}

} // namespace tut
