/*
 * InputConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/InputConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/InputConfig");
} // unnamed namespace


namespace tut
{

// test creating valid data
template<>
template<>
void testObj::test<1>(void)
{
  const InputConfig::TypeName name("someinput");
  const InputConfig::Options  opts;
  const InputConfig pc(name, opts);
  ensure_equals("invalid type", pc.getType(), "someinput");
}

} // namespace tut
