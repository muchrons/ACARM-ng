/*
 * QueueRestorer.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <cassert>

#include "Core/QueueRestorer.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"

using namespace Core;
using namespace TestHelpers::Persistency;

namespace
{

struct TestRestorer: public Persistency::IO::Restorer
{
  explicit TestRestorer(::Persistency::IO::Transaction &t):
    Persistency::IO::Restorer(t)
  {
  }

  virtual void restoreAllInUseImpl(::Persistency::IO::Transaction &/*t*/, NodesVector &out)
  {
    out.push_back( makeNewNode() );
    out.push_back( makeNewLeaf() );
  }

  virtual void restoreBetweenImpl(::Persistency::IO::Transaction &/*t*/,
                                  NodesVector                    &/*out*/,
                                  const ::Persistency::Timestamp &/*from*/,
                                  const ::Persistency::Timestamp &/*to*/)
  {
    throw std::logic_error("this method should NOT be called here at all");
  }
}; // struct TestRestorer

struct ConnectionTestRestorer: public ConnectionUserStubBase
{
  virtual ::Persistency::IO::RestorerAutoPtr restorerImpl(::Persistency::IO::Transaction &t)
  {
    return ::Persistency::IO::RestorerAutoPtr( new TestRestorer(t) );
  }
}; // struct ConnectionTestRestorer


struct TestClass
{
  TestClass(void):
    conn_(new ConnectionTestRestorer)
  {
  }

  Core::Types::SignedNodesFifo     queue_;
  Persistency::IO::ConnectionPtrNN conn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/QueueRestorer");
} // unnamed namespace


namespace tut
{

// test reading some elements
template<>
template<>
void testObj::test<1>(void)
{
  QueueRestorer rq(conn_, queue_);
  ensure_equals("invalid number of elements restored", queue_.size(), 2u);
}

} // namespace tut
