/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"

using namespace Persistency::IO::Postgres::detail;
using namespace std;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Restorer::Restorer(Transaction    &t,
                   DBHandlerPtrNN  dbHandler):
  IO::Restorer(t),
  dbHandler_(dbHandler)
{
}

void Restorer::restoreAllInUseImpl(Transaction &t, NodesVector &out)
{
  EntryReader er(t, *dbHandler_);
  vector<DataBaseID> maInUse( er.readIDsMalertsInUse() );
  Restore(er, out, maInUse);
}

void Restorer::restoreBetweenImpl(Transaction     &/*t*/,
                                  NodesVector     &/*out*/,
                                  const Timestamp &/*from*/,
                                  const Timestamp &/*to*/)
{
  // TODO
  // EntryReader er(t, *dbHandler_);
  // vector<DataBaseID> maBetween( er.readIDsMalertsBetween(from, to) );
  // Restore(er, out, maBetween);
}

BackendFactory::FactoryPtr Restorer::createStubIO(void)
{
  const BackendFactory::FactoryTypeName name("stubx");
  const BackendFactory::Options         options;
  return BackendFactory::create(name, options);
}


TreePtr Restorer::getNode(Tree::IDNode id )
{
  if(treeNodes_.count(id) > 0)
    return treeNodes_.find(id)->second;
  else
    // TODO: use typedef for this
    return boost::shared_ptr<Tree>();
}

int Restorer::getNumberOfChildren(Tree::IDNode id )
{
  if(treeNodes_.count(id) > 0)
    return treeNodes_.find(id)->second->getChildrenNumber();
  else
    return 0;
}

GraphNodePtrNN Restorer::DeepFirstSearch(Tree::IDNode          id,
                                         NodesVector          &out,
                                         EntryReader          &er,
                                         IO::ConnectionPtrNN   connStubIO,
                                         IO::Transaction      &tStubIO)
{
  TreePtr node = getNode(id);
  // check if there are no children (i.e. is leaf)
  if( node == NULL )
  {
    return GraphNodePtrNN( new GraphNode( er.getLeaf(id), connStubIO, tStubIO ) );
  }
  vector<GraphNodePtrNN> tmpNodes;
  vector<Tree::IDNode>   nodeChildren( node->getChildren() );
  for(vector<Tree::IDNode>::iterator it = nodeChildren.begin();
      it != nodeChildren.end(); ++it)
  {
    tmpNodes.push_back( DeepFirstSearch( *it, out, er, connStubIO, tStubIO ) );
  }
  // TODO: this cannot be assert - it has to be runtime-check, since invalid
  //       data may appear in data base for some reason.
  assert(tmpNodes.size() >= 2);
  NodeChildrenVector vec(tmpNodes[0], tmpNodes[1]);
  // TODO: note that you can use vector<>::reserve() method to ensure no extra
  //       allocations will be done when adding new elements.
  for(unsigned int i = 2; i<tmpNodes.size(); ++i)
  {
    vec.push_back(tmpNodes[i]);
  }
  GraphNodePtrNN graphNode(new GraphNode( er.readMetaAlert(id),
                                          connStubIO, tStubIO, vec ));
  out.push_back(graphNode);
  return graphNode;
}

void Restorer::Restore(EntryReader &er, NodesVector &/*out*/, vector<DataBaseID> &malerts)
{

  for(vector<DataBaseID>::iterator it = malerts.begin(); it != malerts.end(); ++it)
  {
    vector<DataBaseID> malertChildren( er.readMetaAlertChildren( (*it) ) );
    // put this data to the tree which represents meta alerts tree structure
    pair<Tree::IDNode, TreePtr> tmp(*it, TreePtr(new Tree(*it, malertChildren) ) );
    treeNodes_.insert(tmp);
  }

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction tStubIO( connStubIO->createNewTransaction("stub transaction") );

  // vector<DataBaseID> roots( er.readRoots());
  // for(vector<DataBaseID>::iteratot it = roots.begin(); it != roots.end(); ++it)
  // {
  //   out.push_back( DeepFirstSearch(*it, out, er, connStubIO, tStubIO) );
  // }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
