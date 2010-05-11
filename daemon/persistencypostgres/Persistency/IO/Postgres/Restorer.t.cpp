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
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("restore_tests") )
  {
    tdba_.removeAllData();
  }

  // TODO: 'out' should be const-ref.
  void checkCache(Restorer::NodesVector &out) const
  {
    for(Restorer::NodesVector::iterator it = out.begin(); it !=out.end(); ++it)
    {
      if( (*it)->isLeaf() )
      {
        tut::ensure("alert shoud be in cache", idCache_->has( (*it)->getAlert()) );
        const Alert::ReportedHosts sourceHosts = (*it)->getAlert()->getReportedSourceHosts();
        const Alert::ReportedHosts targetHosts = (*it)->getAlert()->getReportedTargetHosts();
        for(Alert::ReportedHosts::const_iterator hi = sourceHosts.begin(); hi != sourceHosts.end(); ++hi)
          tut::ensure("alert shoud be in cache", idCache_->has( (*hi) ) );
        for(Alert::ReportedHosts::const_iterator hi = targetHosts.begin(); hi != targetHosts.end(); ++hi)
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
    // put tree in vector

    tut::ensure_equals("invalid size", out.size(), outVec.size());
    tut::ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
    // check if restored alerts and meta alerts exist in cache
    checkCache(out);
    t_.commit();
  }

  // TODO: this method does more than checking - consider name like removeAndCheck()
  void check(const Restorer::NodesVector &outVec, const std::string &node, const std::string &child)
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
    GraphNodePtrNN leaf1 = makeNewLeaf("leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf("leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);
    GraphNodePtrNN node3 = makeNewNode(leaf1, leaf2, "node3");
    first.push_back(node3);

    GraphNodePtrNN leaf3 = makeNewLeaf("leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf("leaf4");
    second.push_back(leaf3);
    second.push_back(leaf4);
    GraphNodePtrNN node4 = makeNewNode(leaf3, leaf4, "node4");
    second.push_back(node4);

    GraphNodePtrNN leaf5 = makeNewLeaf("leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf("leaf6");
    second.push_back(leaf5);
    second.push_back(leaf6);
    GraphNodePtrNN node5 = makeNewNode(leaf5, leaf6, "node5");

    second.push_back(node5);

    GraphNodePtrNN node1 = makeNewNode(node3, node4, "node1");
    second.push_back(node1);


    GraphNodePtrNN node2 = makeNewNode(node4, node5, "node2");
    second.push_back(node2);

    GraphNodePtrNN root = makeNewNode(node1, node2, "root");
    second.push_back(root);
  }

  //
  //                  root1                      root2
  //                        node2          node3       node4
  //                     leaf3  leaf4   leaf5 leaf6 leaf7  leaf8
  //
  void makeNewTreeB(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf("leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf("leaf2");
    first.push_back(leaf1);
    first.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(leaf1,
        leaf2,
        "node1");
    first.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf("leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf("leaf4");
    second.push_back(leaf3);
    second.push_back(leaf4);

    GraphNodePtrNN node2 = makeNewNode(leaf3, leaf4, "node2");
    second.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN leaf5 = makeNewLeaf("leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf("leaf6");

    first.push_back(leaf5);
    first.push_back(leaf6);

    GraphNodePtrNN node3 = makeNewNode(leaf5, leaf6, "node3");

    first.push_back(node3);

    GraphNodePtrNN leaf7 = makeNewLeaf("leaf7");
    GraphNodePtrNN leaf8 = makeNewLeaf("leaf8");

    first.push_back(leaf7);
    first.push_back(leaf8);

    GraphNodePtrNN node4 = makeNewNode(leaf7, leaf8, "node4");

    first.push_back(node4);

    GraphNodePtrNN root2 = makeNewNode(node3, node4, "root2");

    first.push_back(root2);

  }

  //
  //                  root1                      root2
  //             node1                     node3       node4
  //          leaf1 leaf2               leaf5 leaf6 leaf7  leaf8
  //
  void makeNewTreeC(Restorer::NodesVector &first, Restorer::NodesVector &second)
  {
    GraphNodePtrNN leaf1 = makeNewLeaf("leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf("leaf2");
    second.push_back(leaf1);
    second.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(leaf1, leaf2, "node1");
    second.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf("leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf("leaf4");
    first.push_back(leaf3);
    first.push_back(leaf4);

    GraphNodePtrNN node2 = makeNewNode(leaf3, leaf4, "node2");
    first.push_back(node2);

    GraphNodePtrNN root1 = makeNewNode(node1, node2, "root1");
    second.push_back(root1);

    GraphNodePtrNN leaf5 = makeNewLeaf("leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf("leaf6");

    first.push_back(leaf5);
    first.push_back(leaf6);

    GraphNodePtrNN node3 = makeNewNode(leaf5, leaf6, "node3");

    first.push_back(node3);

    GraphNodePtrNN leaf7 = makeNewLeaf("leaf7");
    GraphNodePtrNN leaf8 = makeNewLeaf("leaf8");

    first.push_back(leaf7);
    first.push_back(leaf8);

    GraphNodePtrNN node4 = makeNewNode(leaf7, leaf8, "node4");

    first.push_back(node4);

    GraphNodePtrNN root2 = makeNewNode(node3, node4, "root2");

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
    GraphNodePtrNN leaf1 = makeNewLeaf("leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf("leaf2");
    vec.push_back(leaf1);
    vec.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(leaf1, leaf2, "node1");
    vec.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf("leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf("leaf4");
    vec.push_back(leaf3);
    vec.push_back(leaf4);

    GraphNodePtrNN node3 = makeNewNode(leaf3, leaf4, "node3");
    vec.push_back(node3);
    GraphNodePtrNN leaf5 = makeNewLeaf("leaf5");
    vec.push_back(leaf5);
    GraphNodePtrNN node2 = makeNewNode(node3, leaf5, "node2");
    vec.push_back(node2);
    GraphNodePtrNN root = makeNewNode(node1, node2, "root");
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
    GraphNodePtrNN leaf1 = makeNewLeaf("leaf1");
    GraphNodePtrNN leaf2 = makeNewLeaf("leaf2");
    vec.push_back(leaf1);
    vec.push_back(leaf2);

    GraphNodePtrNN node1 = makeNewNode(leaf1, leaf2, "node1");
    vec.push_back(node1);

    GraphNodePtrNN leaf3 = makeNewLeaf("leaf3");
    GraphNodePtrNN leaf4 = makeNewLeaf("leaf4");
    vec.push_back(leaf3);
    vec.push_back(leaf4);

    GraphNodePtrNN node3 = makeNewNode(leaf3, leaf4, "node3");
    vec.push_back(node3);
    GraphNodePtrNN leaf5 = makeNewLeaf("leaf5");
    GraphNodePtrNN leaf6 = makeNewLeaf("leaf6");
    vec.push_back(leaf5);
    vec.push_back(leaf6);
    GraphNodePtrNN node4 = makeNewNode(leaf5, leaf6, "node4");
    vec.push_back(node4);

    GraphNodePtrNN node2 = makeNewNode(node3, node4, "node2");
    vec.push_back(node2);
    GraphNodePtrNN root = makeNewNode(node1, node2, "root");
    vec.push_back(root);
    return vec;
  }

  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Restorer");
} //unnamned namespace

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
  t_.commit();
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
  check(outVec, "node1", "node3");
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
  check(outVec, "root1", "node1");
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
  check(outVec, "root1", "node2");
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

// TODO: try restoring valid data with restore(..., from, to) where some sub-tree part
//       is in use, but does not fit into [from, to] range.

} // namespace tut
