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

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace std;

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

  void deepSearch(GraphNodePtrNN node, std::vector<GraphNodePtrNN> &outVec)
  {
    if( node->isLeaf())
    {
      outVec.push_back(node);
    }
    else
    {
      outVec.push_back(node);
      for(GraphNode::iterator it = node->begin(); it != node->end(); ++it)
      {
        deepSearch(*it, outVec);
      }
    }
    return;
  }
  void checkCache(std::vector<GraphNodePtrNN> &out)
  {
    for(std::vector<GraphNodePtrNN>::iterator it = out.begin(); it !=out.end(); ++it)
    {
      if( (*it)->isLeaf() )
        tut::ensure("alert shoud be in cache", idCache_->has( (*it)->getAlert()) );
      else
        tut::ensure("meta alert shoud be in cache", idCache_->has( (*it)->getMetaAlert()) );
    }
  }
  TestDBAccess            tdba_;
  IDCachePtrNN            idCache_;
  DBHandlerPtrNN          dbh_;
  IO::ConnectionPtrNN     conn_;
  Transaction             t_;

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
  std::vector<GraphNodePtrNN> out;
  std::vector<GraphNodePtrNN> outVec;
  // create tree and save data to the data base
  GraphNodePtrNN tree = makeNewTree1();
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  // put tree in vector
  deepSearch(tree, outVec);

  ensure_equals("invalid size", out.size(), outVec.size());
  ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
  // check if restored alerts and meta alerts exist in cache
  checkCache(out);
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
  std::vector<GraphNodePtrNN> out;
  std::vector<GraphNodePtrNN> outVec = makeNewTree3();
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  ensure_equals("invalid size", out.size(), outVec.size());
  ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
  // check if restored alerts and meta alerts exist in cache
  checkCache(out);
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
  std::vector<GraphNodePtrNN> out;
  std::vector<GraphNodePtrNN> outVec;
  // create tree and save data to the data base
  GraphNodePtrNN tree = makeNewTree4();
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  // put tree in vector
  deepSearch(tree, outVec);

  ensure_equals("invalid size", out.size(), outVec.size());
  ensure("vectors are different", Commons::ViaUnorderedCollection::equal(out, outVec) );
  // check if restored alerts and meta alerts exist in cache
  checkCache(out);
}

// try restoring empty set
template<>
template<>
void testObj::test<4>(void)
{
  std::vector<GraphNodePtrNN> out;
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  ensure_equals("invalid size", out.size(), 0);
}

// TODO: try restoring invalid data (i.e. node that has no children, etc...)
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
