/*
 * TriggerConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/TriggerConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/TriggerConfig");
} // unnamed namespace


namespace tut
{

// test creating valid data
template<>
template<>
void testObj::test<1>(void)
{
  const TriggerConfig::TypeName type("sometrigger");
  const TriggerConfig::Options  opts;
  const TriggerConfig           pc(type, opts);
  ensure_equals("invalid type", pc.getType(), "sometrigger");
  ensure("preprocessor is set", pc.getPreprocessorConfig()==NULL);
}

// test creating valid data, with preprocessor
template<>
template<>
void testObj::test<2>(void)
{
  const TriggerConfig::TypeName type("sometrigger");
  const TriggerConfig::Options  opts;
  const Preprocessor::Config    ppCfg;
  const TriggerConfig           pc(type, opts, ppCfg);
  ensure("preprocessor not set", pc.getPreprocessorConfig()!=NULL);
}

} // namespace tut
