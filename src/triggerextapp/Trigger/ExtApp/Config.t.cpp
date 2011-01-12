/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/ExtApp/Config.hpp"

using namespace Trigger::ExtApp;

namespace
{

struct TestClass
{
  TestClass(void):
    cfg_( "/bin/true", Trigger::Simple::ThresholdConfig("1.2", "4") )
  {
  }

  void ensureThrows(const boost::filesystem::path &p)
  {
    try
    {
      Config c(p, cfg_.getThresholdConfig() );      // should throw
      tut::fail("config didn't failed on invalid path");
    }
    catch(const Exception &)
    {
      // this is expected
    }
  }

  const Config cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/ExtApp/Config");
} // unnamed namespace


namespace tut
{

// test application path
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid path", cfg_.getPath(), "/bin/true");
}

// get thresholds
template<>
template<>
void testObj::test<2>(void)
{
  ensure("severity threshold is NULL", cfg_.getThresholdConfig().getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity threshold", *cfg_.getThresholdConfig().getSeverityThreshold().get(), 1.2);

  ensure("alerts' count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid alerts' count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 4);
}

// test copyability
template<>
template<>
void testObj::test<3>(void)
{
  const Config tmp=cfg_;
  ensure_equals("configs do not match", tmp.getPath(), cfg_.getPath() );
}

// test exception when file does not exist
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrows("/i/do/not/exist");
}

// test exception when file is not sane
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrows("testdata/symlink");
}

} // namespace tut
