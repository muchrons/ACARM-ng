/*
 * Downloader.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Filter/IPBlackList/Downloader.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/Downloader");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Downloader d(10);
}

// test test default c-tor
template<>
template<>
void testObj::test<2>(void)
{
  Downloader d;
}

// test getting few sample entries.
template<>
template<>
void testObj::test<3>(void)
{
  const Downloader d(12);
  ensure("string too short", d.get().length()>50 );
}

} // namespace tut
