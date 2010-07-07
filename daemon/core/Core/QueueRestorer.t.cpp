/*
 * QueueRestorer.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <cassert>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/QueueRestorer.hpp"
#include "TestHelpers/Persistency/TestPostgres.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestDBAccess.hpp"

using namespace Core;

namespace
{
// TODO: this has to be fixed - non-postgres components cannot use postgres for tests!
//       consider overloading some parts of the Persistency::Stubs according to needs.
struct TestClass: public TestHelpers::Persistency::TestPostgres
{
  TestClass(void)
  {
    tdba_.removeAllData();
  }

  Core::Types::UniqueNodesFifo           queue_;
  TestHelpers::Persistency::TestDBAccess tdba_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/QueueRestorer");
} // unnamed namespace


namespace tut
{

// test reading 0 elements
template<>
template<>
void testObj::test<1>(void)
{
  QueueRestorer rq(queue_);
  ensure_equals("some elements have been read", queue_.size(), 0u);
}

// test reading more elements from data base
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::GraphNodePtr node=TestHelpers::Persistency::makeNewNode();
  QueueRestorer             rq(queue_);
  ensure_equals("invalid elements count has been read", queue_.size(), 3u);
}

// test reading from data base when some elements are unused
template<>
template<>
void testObj::test<3>(void)
{
  TestHelpers::Persistency::makeNewNode();
  tdba_.markAllUnused();
  Persistency::GraphNodePtr node=TestHelpers::Persistency::makeNewNode();
  QueueRestorer             rq(queue_);
  ensure_equals("invalid elements count has been read", queue_.size(), 3u);
}

} // namespace tut
