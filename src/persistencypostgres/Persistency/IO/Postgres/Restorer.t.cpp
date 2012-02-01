/*
 * Restorer.t.cpp
 *
 */
#include <tut.h>

#include "Base/ViaPointer.hpp"
#include "Commons/ViaUnorderedCollection.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/ExceptionBadNumberOfNodeChildren.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;
using namespace std;
using namespace pqxx;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_(DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("restore_tests") )
  {
  }

  void checkCache(Restorer::NodesVector &out) const
  {
    for(Restorer::NodesVector::iterator it = out.begin(); it !=out.end(); ++it)
    {
      if( (*it)->isLeaf() )
      {
        tut::ensure("alert shoud be in cache", idCache_->has( (*it)->getAlert() ) );
        const Alert::Hosts sourceHosts = (*it)->getAlert()->getSourceHosts();
        const Alert::Hosts targetHosts = (*it)->getAlert()->getTargetHosts();
        for(Alert::Hosts::const_iterator hi = sourceHosts.begin(); hi != sourceHosts.end(); ++hi)
          tut::ensure("alert shoud be in cache", idCache_->has( (*hi) ) );
        for(Alert::Hosts::const_iterator hi = targetHosts.begin(); hi != targetHosts.end(); ++hi)
          tut::ensure("alert shoud be in cache", idCache_->has( (*hi) ) );
      }
      else
        tut::ensure("meta alert shoud be in cache", idCache_->has( (*it)->getMetaAlert()) );
    }
  }

  void check(const Restorer::NodesVector &outVec)
  {
    Restorer::NodesVector out;
    // create restorer
    Restorer r(t_, dbh_);
    // restore data from data base
    r.restoreAllInUse(out);
    t_.commit();
    // put tree in vector
    tut::ensure_equals("invalid size", out.size(), outVec.size());
    tut::ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
    // check if restored alerts and meta alerts exist in cache
    checkCache(out);
  }

  void checkBetween(const Restorer::NodesVector &outVec, const Timestamp &from, const Timestamp &to)
  {
    Restorer::NodesVector out;
    // create restorer
    Restorer r(t_, dbh_);
    // restore data from data base
    r.restoreBetween(out, from, to);
    t_.commit();
    // put tree in vector

    tut::ensure_equals("invalid size", out.size(), outVec.size());
    tut::ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
    // check if restored alerts and meta alerts exist in cache
    checkCache(out);
  }

  void removeAndCheck(const Restorer::NodesVector &outVec, const std::string &node, const std::string &child)
  {
    removeNodeConnection(node, child);
    check(outVec);
  }

  //
  //                   root1
  //             node1       node2
  //                   node4       node5
  //                leaf3 leaf4 leaf5 leaf6
  //
  void makeNewTreeA(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);
    GraphNodePtrNN node3 = makeNewNode(102u, leaf1, leaf2, "node3");
    first.push_back(node3);

    GraphNodePtrNN leaf3 = makeNewLeaf(104u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(105u, "leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);
    GraphNodePtrNN node4 = makeNewNode(106u, leaf3, leaf4, "node4");
    first.push_back(node4);

    GraphNodePtrNN leaf5 = makeNewLeaf(107u, "leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf(108u, "leaf6");
    first.push_back(leaf5);
    first.push_back(leaf6);
    GraphNodePtrNN node5 = makeNewNode(109u, leaf5, leaf6, "node5");

    first.push_back(node5);

    GraphNodePtrNN node1 = makeNewNode(110u, node3, node4, "node1");
    second.push_back(node1);


    GraphNodePtrNN node2 = makeNewNode(111u, node4, node5, "node2");
    first.push_back(node2);

    GraphNodePtrNN root = makeNewNode(112u, node1, node2, "root");
    second.push_back(root);
  }

  //
  //                  root1                      root2
  //                        node2          node3       node4
  //                     leaf3  leaf4   leaf5 leaf6 leaf7  leaf8
  //
  void makeNewTreeB(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);

    GraphNodePtrNN node2 = makeNewNode(105u, leaf3, leaf4, "node2");
    first.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(106u, node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN leaf5 = makeNewLeaf(107u, "leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf(108u, "leaf6");

    first.push_back(leaf5);
    first.push_back(leaf6);

    GraphNodePtrNN node3 = makeNewNode(109u, leaf5, leaf6, "node3");

    first.push_back(node3);

    GraphNodePtrNN leaf7 = makeNewLeaf(110u, "leaf7");
    GraphNodePtrNN leaf8 = makeNewLeaf(111u, "leaf8");

    first.push_back(leaf7);
    first.push_back(leaf8);

    GraphNodePtrNN node4 = makeNewNode(112u, leaf7, leaf8, "node4");

    first.push_back(node4);

    GraphNodePtrNN root2 = makeNewNode(113u, node3, node4, "root2");

    first.push_back(root2);

  }

  //
  //                  root1                      root2
  //             node1                     node3       node4
  //          leaf1 leaf2               leaf5 leaf6 leaf7  leaf8
  //
  void makeNewTreeC(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);

    GraphNodePtrNN node2 = makeNewNode(105u, leaf3, leaf4, "node2");
    first.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(106u, node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN leaf5 = makeNewLeaf(107u, "leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf(108u, "leaf6");

    first.push_back(leaf5);
    first.push_back(leaf6);

    GraphNodePtrNN node3 = makeNewNode(109u, leaf5, leaf6, "node3");

    first.push_back(node3);

    GraphNodePtrNN leaf7 = makeNewLeaf(110u, "leaf7");
    GraphNodePtrNN leaf8 = makeNewLeaf(111u, "leaf8");

    first.push_back(leaf7);
    first.push_back(leaf8);

    GraphNodePtrNN node4 = makeNewNode(112u, leaf7, leaf8, "node4");

    first.push_back(node4);

    GraphNodePtrNN root2 = makeNewNode(113u, node3, node4, "root2");

    first.push_back(root2);

  }

  //
  //                root
  //     node1               node2
  //  leaf1 leaf2      node3      leaf5
  //                leaf3 leaf4
  //
  Restorer::NodesVector makeNewTreeD(void)
  {
    Restorer::NodesVector vec;
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    vec.push_back(leaf1);
    vec.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    vec.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    vec.push_back(leaf3);
    vec.push_back(leaf4);

    GraphNodePtrNN node3 = makeNewNode(105u, leaf3, leaf4, "node3");
    vec.push_back(node3);
    GraphNodePtrNN leaf5 = makeNewLeaf(106u, "leaf5");
    vec.push_back(leaf5);
    GraphNodePtrNN node2 = makeNewNode(107u, node3, leaf5, "node2");
    vec.push_back(node2);
    GraphNodePtrNN root = makeNewNode(108u, node1, node2, "root");
    vec.push_back(root);
    return vec;
  }

  //
  //                root
  //     node1               node2
  //  leaf1 leaf2      node3       node4
  //                leaf3 leaf4 leaf5 leaf6
  //
  Restorer::NodesVector makeNewTreeE(void)
  {
    Restorer::NodesVector vec;
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    vec.push_back(leaf1);
    vec.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    vec.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    vec.push_back(leaf3);
    vec.push_back(leaf4);

    GraphNodePtrNN node3 = makeNewNode(105u, leaf3, leaf4, "node3");
    vec.push_back(node3);
    GraphNodePtrNN leaf5 = makeNewLeaf(106u, "leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf(107u, "leaf6");
    vec.push_back(leaf5);
    vec.push_back(leaf6);
    GraphNodePtrNN node4 = makeNewNode(108u, leaf5, leaf6, "node4");
    vec.push_back(node4);

    GraphNodePtrNN node2 = makeNewNode(109u, node3, node4, "node2");
    vec.push_back(node2);
    GraphNodePtrNN root = makeNewNode(110u, node1, node2, "root");
    vec.push_back(root);
    return vec;
  }

  //
  //                root
  //     node1               node2
  //  leaf1 leaf2      node3       node4
  //                leaf3 leaf4 leaf5 leaf6
  //
  Restorer::NodesVector makeNewTreeF(void)
  {
    Restorer::NodesVector vec;
    const Timestamp t1( timestampFromString("1999-10-10 17:56:07") );
    const Timestamp t2( timestampFromString("2010-04-22 07:56:07") );

    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1", t1);
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2", t1);
    vec.push_back(leaf1);
    vec.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1", t1);
    vec.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3", t2);
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4", t2);
    vec.push_back(leaf3);
    vec.push_back(leaf4);

    GraphNodePtrNN node3 = makeNewNode(105u, leaf3, leaf4, "node3", t2);
    vec.push_back(node3);
    GraphNodePtrNN leaf5 = makeNewLeaf(106u, "leaf5", t1);
    GraphNodePtrNN leaf6 = makeNewLeaf(107u, "leaf6", t1);
    vec.push_back(leaf5);
    vec.push_back(leaf6);
    GraphNodePtrNN node4 = makeNewNode(108u, leaf5, leaf6, "node4", t1);
    vec.push_back(node4);

    GraphNodePtrNN node2 = makeNewNode(109u, node3, node4, "node2", t1);
    vec.push_back(node2);
    GraphNodePtrNN root = makeNewNode(110u, node1, node2, "root", t1);
    vec.push_back(root);
    return vec;
  }

  //
  //                  root1                      root2
  //             node1                     node3       node4
  //          leaf1 leaf2               leaf5 leaf6 leaf7  leaf8
  //
  void makeNewTreeG(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    const Timestamp t1( timestampFromString("1999-10-10 17:56:07") );
    const Timestamp t2( timestampFromString("2010-01-22 07:56:07") );
    const Timestamp t3( timestampFromString("2010-04-22 07:56:07") );
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1", t2);
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2", t2);
    first.push_back(leaf1);
    first.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1", t2);
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3", t1);
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4", t1);
    second.push_back(leaf3);
    second.push_back(leaf4);

    GraphNodePtrNN node2 = makeNewNode(105u, leaf3, leaf4, "node2", t1);
    second.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(106u, node1, node2, "root1", t2);
    second.push_back(root1);

    GraphNodePtrNN leaf5 = makeNewLeaf(107u, "leaf5", t3);
    GraphNodePtrNN leaf6 = makeNewLeaf(108u, "leaf6", t3);

    first.push_back(leaf5);
    first.push_back(leaf6);

    GraphNodePtrNN node3 = makeNewNode(109u, leaf5, leaf6, "node3", t3);

    first.push_back(node3);

    GraphNodePtrNN leaf7 = makeNewLeaf(110u, "leaf7", t3);
    GraphNodePtrNN leaf8 = makeNewLeaf(111u, "leaf8", t3);

    first.push_back(leaf7);
    first.push_back(leaf8);

    GraphNodePtrNN node4 = makeNewNode(112u, leaf7, leaf8, "node4", t3);

    first.push_back(node4);

    GraphNodePtrNN root2 = makeNewNode(113u, node3, node4, "root2", t3);

    first.push_back(root2);
  }

  //
  //                   root1       root2
  //             node1                   node3
  //       leaf1       leaf2       leaf3       leaf4
  //
  void makeNewTreeH(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);
    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);
    GraphNodePtrNN node3 = makeNewNode(105u, leaf3, leaf4, "node3");
    first.push_back(node3);

    GraphNodePtrNN node2 = makeNewNode(106u, leaf2, leaf3, "node2");
    first.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(107u, node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN root2 = makeNewNode(108u, node2, node3, "root2");
    first.push_back(root2);
  }

  //
  //                   root1       root2
  //             node1                   node3
  //       leaf1       leaf2       leaf3       leaf4
  //
  void makeNewTreeI(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf(100u, "leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf(101u, "leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);
    GraphNodePtrNN node1 = makeNewNode(102u, leaf1, leaf2, "node1");
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf(103u, "leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf(104u, "leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);
    GraphNodePtrNN node3 = makeNewNode(105u, leaf3, leaf4, "node3");
    first.push_back(node3);

    GraphNodePtrNN node2 = makeNewNode(106u, leaf2, leaf3, "node2");
    first.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(107u, node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN root2 = makeNewNode(108u, node2, node3, "root2");
    second.push_back(root2);
  }

  DataCleaner         dc_;
  IDCachePtrNN        idCache_;
  DBHandlePtrNN       dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Restorer");
} // unnamned namespace

namespace tut
{

// trying restore all in use
//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
template<>
template<>
void testObj::test<1>(void)
{
  // create tree and save data to the data base
  const Restorer::NodesVector outVec = makeNewTreeD();
  check(outVec);
}

// trying restoring tree
//
//                   root1       root2
//             node1       node2       node3
//       leaf1       leaf2       leaf3       leaf4
//
template<>
template<>
void testObj::test<2>(void)
{
  const Restorer::NodesVector outVec = makeNewTree3();
  check(outVec);
}

// trying restoring tree
//
//                root
//     node1               node2
//  leaf1 leaf2      node3       node4
//                leaf3 leaf4 leaf5 leaf6
//
template<>
template<>
void testObj::test<3>(void)
{
  // create tree and save data to the data base
  const Restorer::NodesVector outVec = makeNewTreeE();
  check(outVec);
}

// try restoring empty set
template<>
template<>
void testObj::test<4>(void)
{
  Restorer::NodesVector out;
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  ensure_equals("invalid size", out.size(), 0);
}

// trying restoring tree
//
//                   root1
//             node1       node2
//       node3       node4       node5
//    leaf1 leaf2 leaf3 leaf4 leaf5 leaf6
//
template<>
template<>
void testObj::test<5>(void)
{
  const Restorer::NodesVector outVec = makeNewTree5();
  check(outVec);
}

// try restoring invalid data
//
//                   root1
//             node1       node2
//                   node4       node5
//                leaf3 leaf4 leaf5 leaf6
//
template<>
template<>
void testObj::test<6>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeA(outVec, tmp);
  removeAndCheck(outVec, "node1", "node3");
  t_.commit();
}

// try restoring invalid data
//
//                  root1                      root2
//                        node2          node3       node4
//                     leaf3  leaf4   leaf5 leaf6 leaf7  leaf8
//
template<>
template<>
void testObj::test<7>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeB(outVec, tmp);
  removeAndCheck(outVec, "root1", "node1");
  t_.commit();
}

// try restoring invalid data
//
//                  root1                      root2
//             node1                     node3       node4
//          leaf1 leaf2               leaf5 leaf6 leaf7  leaf8
//
template<>
template<>
void testObj::test<8>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeC(outVec, tmp);
  removeAndCheck(outVec, "root1", "node2");
  t_.commit();
}

//
//     node1
//  leaf1  leaf2
//
template<>
template<>
void testObj::test<9>(void)
{
  const Restorer::NodesVector outVec = makeNewTree7();
  check(outVec);
}

// trying restoring between
template<>
template<>
void testObj::test<10>(void)
{
  const Restorer::NodesVector outVec = makeNewTreeF();
  const Timestamp from( timestampFromString("1999-10-10 17:56:07") );
  const Timestamp to( timestampFromString("2010-04-22 07:56:07") );
  checkBetween(outVec, from, to);
}

// try restoring empty set
template<>
template<>
void testObj::test<11>(void)
{
  Restorer::NodesVector out;
  makeNewTreeF();
  const Timestamp from( timestampFromString("2011-10-10 17:56:07") );
  const Timestamp to( timestampFromString("2012-04-22 07:56:07") );
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreBetween(out, from, to);
  ensure_equals("invalid size", out.size(), 0);
}

// try restoring valid data with restore(..., from, to) where some sub-tree part
// is in use, but does not fit into [from, to] range.
template<>
template<>
void testObj::test<12>(void)
{
  const Timestamp from( timestampFromString("2010-01-22 07:56:07") );
  const Timestamp to( timestampFromString("2010-04-22 07:56:07") );
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeG(outVec, tmp);
  checkBetween(outVec, from, to);
}

// try restoring invalid data
//
//                   root1
//             node1       node2
//                   node4       node5
//                leaf3 leaf4 leaf5 leaf6
//
template<>
template<>
void testObj::test<13>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeA(outVec, tmp);
  removeAndCheck(outVec, "node1", "node3");
  t_.commit();
  ensure_equals("invalid number of meta alerts in use", getNoOfMetaAlertsInUse(), 10u);
}

//
//        node1 (102)
//  leaf1 (100)  leaf2 (101)
//

// test restoring MetaAlert::ID() for node/leaf
template<>
template<>
void testObj::test<14>(void)
{
  const Restorer::NodesVector outVec = makeNewTree7();
  check(outVec);
  for(Restorer::NodesVector::const_iterator it = outVec.begin(); it != outVec.end(); ++it)
  {
    if( (*it)->isLeaf() )
    {
      // test restoring MetaAlert::ID() for leaf
      const std::string name( (*it)->getMetaAlert()->getName().get() );
      if(name == "leaf1")
        ensure_equals("invalid leaf ID restored",(*it)->getMetaAlert()->getID().get() ,100u);
      else
        ensure_equals("invalid leaf ID restored",(*it)->getMetaAlert()->getID().get() ,101u);
    }
    else
    {
      // test restoring MetaAlert::ID() for node
      ensure_equals("invalid node ID restored",(*it)->getMetaAlert()->getID().get() ,102u);
    }
  }
}

// trying restoring one leaf
template<>
template<>
void testObj::test<15>(void)
{
  Restorer::NodesVector outVec;
  const GraphNodePtrNN leaf = makeNewLeaf(100u, "leaf");
  check(outVec);
}

/*
              root1   root2
              /       /   \
           node1  node2   node3
           /   \  /   \   /   \
        leaf1  leaf2  leaf3  leaf4
*/
template<>
template<>
void testObj::test<16>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeH(outVec, tmp);
  removeAndCheck(outVec, "root1", "node2");
  t_.commit();
}

/*
              root1   root2
              /           \
           node1  node2   node3
           /   \  /   \   /   \
        leaf1  leaf2  leaf3  leaf4
*/
template<>
template<>
void testObj::test<17>(void)
{
  Restorer::NodesVector outVec;
  Restorer::NodesVector tmp;
  makeNewTreeI(outVec, tmp);
  removeNodeConnection("root2", "node2");
  removeNodeConnection("root1", "node2");
  check(outVec);
  t_.commit();
}

} // namespace tut
