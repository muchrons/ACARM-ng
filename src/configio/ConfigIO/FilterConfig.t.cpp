/*
 * FilterConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/FilterConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/FilterConfig");
} // unnamed namespace


namespace tut
{

// test creating valid data
template<>
template<>
void testObj::test<1>(void)
{
  const FilterConfig::TypeName name("somefilter");
  const FilterConfig::Options  opts;
  const FilterConfig pc(name, opts);
  ensure_equals("invalid type", pc.getType(), "somefilter");
}

} // namespace tut
