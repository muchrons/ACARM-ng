/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Python/Config.hpp"

using namespace Trigger::Python;

namespace
{

struct TestClass
{
  TestClass(void):
    th_("1", "2"),
    path_("some/path"),
    cfg_(th_, path_)
  {
  }

  const Trigger::Simple::ThresholdConfig th_;
  const Config::Path                     path_;
  const Config                           cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Python/Config");
} // unnamed namespace


namespace tut
{

// test getting threshold values
template<>
template<>
void testObj::test<1>(void)
{
  ensure("count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL);
  ensure_equals("invalid count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 2);
}

// test getting path
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid path", cfg_.getScriptPath(), "some/path");
}

} // namespace tut
