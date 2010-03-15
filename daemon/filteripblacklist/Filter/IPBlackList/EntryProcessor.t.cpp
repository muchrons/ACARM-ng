/*
 * EntryProcessor.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"
#include "Filter/IPBlackList/DShieldParser.hpp"
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
    dsp_(""),
    bl_( dsp_.begin(), dsp_.end() ),
    bp_(conn_, changed_, "testdipblacklist"),
    ep_(&bl_, &bp_, 0.3)
  {
    assert( changed_.size()==0 );
  }

  Persistency::IO::ConnectionPtrNN conn_;
  DShieldParser                    dsp_;
  BlackList                        bl_;
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
