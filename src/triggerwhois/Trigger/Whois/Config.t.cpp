/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Whois/Config.hpp"

using namespace Trigger::Whois;

namespace
{

struct TestClass
{
  TestClass(void):
    cfg_( "/tmp/message.tmpl", Trigger::Simple::ThresholdConfig("1.2", "4") )
  {
  }

  const Config cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Whois/Config");
} // unnamed namespace

namespace tut
{
// test getting message template file path
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid path", cfg_.getTemplateFilePath(), "/tmp/message.tmpl");
}

// get severity threshold
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
}
} // namespace tut
