/*
 * version.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sstream>

#include "ConfigConsts/version.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/version");
} // unnamed namespace


namespace tut
{

// test presence of version numbers
template<>
template<>
void testObj::test<1>(void)
{
  const int tmp=versionMain+versionMajor+versionMinor;
  ensure("ok", tmp>=0);
}

// test version string
template<>
template<>
void testObj::test<2>(void)
{
  std::stringstream ss;
  ss<<"v"<<versionMain<<"."<<versionMajor<<"."<<versionMinor;
  ensure_equals("invalid version string", ss.str(), versionString);
}

} // namespace tut
