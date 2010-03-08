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
  const TriggerConfig::TypeName name("sometrigger");
  const TriggerConfig::Options  opts;
  const TriggerConfig pc(name, opts);
  ensure_equals("invalid type", pc.getType(), "sometrigger");
}

} // namespace tut
