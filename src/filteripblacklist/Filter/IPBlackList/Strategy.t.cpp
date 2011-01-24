/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <set>
#include <string>
#include <cassert>

#include "Filter/IPBlackList/Strategy.hpp"
#include "Filter/IPBlackList/Downloader.hpp"
#include "Filter/IPBlackList/DShieldParser.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    params_(11, 22, 0.1)
  {
  }

  Persistency::GraphNodePtrNN makeNode(void) const
  {
    return makeNewNode( makeNewLeaf("127.0.0.2"), makeNewLeaf("127.0.0.1") );
  }

  Strategy::ChangedNodes changed_;
  Strategy::Parameters   params_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/Strategy");
} // unnamed namespace


namespace tut
{

// test leaf
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s("somename", params_);
  s.process( makeNewLeaf("127.0.0.2"), changed_ );
  ensure_equals("something changed", changed_.size(), 0u);
}

// test node
template<>
template<>
void testObj::test<2>(void)
{
  Strategy s("somename", params_);
  s.process( makeNode(), changed_ );
  ensure_equals("something changed", changed_.size(), 0u);
}

// test if the same host is not updated twice
template<>
template<>
void testObj::test<3>(void)
{
  // note that there is race condition in next two lines - it may happen that between
  // download of blacklisted IPs by strategy and download by donwloader it may change
  // thus there is some (small) probablilty that this test will randomly fail.
  const Downloader    dwnl(1);                                              // just one entry to download
  const DShieldParser dsp( dwnl.download() );                               // parse it
  Strategy            s( "x", Strategy::Parameters(1, 10, 0.5) );           // initiliaze strategy to use
  ensure("no entries parsed", dsp.begin()!=dsp.end() );
  GraphNodePtrNN leaf=makeNewLeaf( (*dsp.begin()).to_string().c_str() );    // make leaf that is blacklisted
  // this host should be blacklisted
  s.process(leaf, changed_);
  ensure_equals("nothing changed", changed_.size(), 1u);
  changed_.clear();
  // now it should NOT be blacklisted, since it has already changed
  s.process(leaf, changed_);
  ensure_equals("the same host has been black-listed twice", changed_.size(), 0u);
}

} // namespace tut
