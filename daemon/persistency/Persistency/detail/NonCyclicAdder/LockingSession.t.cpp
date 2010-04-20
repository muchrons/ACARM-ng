/*
 * LockingSession.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/ReadWriteMutex.hpp"
#include "Persistency/detail/NonCyclicAdder/LockingSession.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace Base::Threads;
using namespace Persistency;
using namespace Persistency::detail;

namespace
{

struct TestClass
{
  TestClass(void):
    ma1_( new MetaAlert( makeNewAlert() ) ),
    ma2_( new MetaAlert( makeNewAlert() ) ),
    conn_(new TestIOConnection),
    t_( conn_->createNewTransaction("gn_test") ),
    leaf_( makeLeaf() )
  {
  }

  GraphNodePtrNN makeLeaf(void)
  {
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), conn_, t_) );
  }

  LockingSession::ReadTryLockPtr makeLock(void)
  {
    return LockingSession::ReadTryLockPtr( new ReadTryLock(mutexRW_) );
  }

  MetaAlertPtrNN      ma1_;
  MetaAlertPtrNN      ma2_;
  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;

  GraphNodePtrNN      leaf_;

  ReadWriteMutex      mutexRW_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/detail/NonCyclicAdder/LockingSession");
} // unnamed namespace


namespace tut
{

// try c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  LockingSession ls;
}

// test getting non-exisiting pointer
template<>
template<>
void testObj::test<2>(void)
{
  const LockingSession ls;
  ensure("non exisiting entry found", ls.hasNode(*leaf_)==false );
}

// test adding and finding given pointer
template<>
template<>
void testObj::test<3>(void)
{
  LockingSession ls;
  ls.addLockedNode( *leaf_, makeLock() );
  ensure("entry not found", ls.hasNode(*leaf_) );
}

} // namespace tut
