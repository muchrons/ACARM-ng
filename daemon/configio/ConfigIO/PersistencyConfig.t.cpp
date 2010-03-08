/*
 * PersistencyConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/PersistencyConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/PersistencyConfig");
} // unnamed namespace


namespace tut
{

// test creating valid data
template<>
template<>
void testObj::test<1>(void)
{
  const PersistencyConfig::TypeName name("persist");
  const PersistencyConfig::Options  opts;
  const PersistencyConfig pc(name, opts);
  ensure_equals("invalid type", pc.getType(), "persist");
}

} // namespace tut
