/*
 * EntryProcessor.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter;
using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
//    cache_(42),
    bp_(conn_, changed_, "testdipblacklist"),
    ep_(/*&cache_, */&bp_)
  {
    assert( changed_.size()==0 );
  }

  Persistency::IO::ConnectionPtrNN conn_;
//  CachedDNS                        cache_;
  BackendProxy::ChangedNodes       changed_;
  BackendProxy                     bp_;
  EntryProcessor                   ep_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/EntryProcessor");
} // unnamed namespace


namespace tut
{

//
template<>
template<>
void testObj::test<1>(void)
{
  // TODO
}

//
template<>
template<>
void testObj::test<2>(void)
{
  // TODO
}

//
template<>
template<>
void testObj::test<3>(void)
{
  // TODO
}

//
template<>
template<>
void testObj::test<4>(void)
{
  // TODO
}

} // namespace tut
