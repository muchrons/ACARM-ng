/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/DNSResolver/Strategy.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::DNSResolver;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    s_( Strategy::Parameters(42) )
  {
  }

  Persistency::GraphNodePtrNN makeNode(void) const
  {
    return makeNewNode( makeNewLeaf("127.0.0.1", NULL, false),
                        makeNewLeaf("127.0.0.1", NULL, false) );
  }

  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/Strategy");
} // unnamed namespace


namespace tut
{

// test leaf
template<>
template<>
void testObj::test<1>(void)
{
  s_.process( makeNewLeaf("127.0.0.1", NULL, false), changed_ );
  ensure_equals("nothing changed", changed_.size(), 1);
}

// test node
template<>
template<>
void testObj::test<2>(void)
{
  s_.process( makeNode(), changed_ );
  ensure_equals("nothing changed", changed_.size(), 2);
}

} // namespace tut
