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

void Restorer::restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to)
{
  // TODO
  EntryReader er(t, *dbHandler_);
  vector<DataBaseID> maBetween( er.readIDsMalertsBetween(from, to) );
  Restore(er, out, maBetween);
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
                                         IO::ConnectionPtrNN  connStubIO,
                                         IO::Transaction      &tStubIO)
{
  TreePtr node = getNode(id);
  // change to check if there is no children
  if( node == NULL )
  {
    return GraphNodePtrNN( new GraphNode( er.getLeaf(id), connStubIO, tStubIO ) );
  }
  vector<GraphNodePtrNN> tmpNodes;
  vector<Tree::IDNode> nodeChildren( node->getChildren() );
  for(vector<Tree::IDNode>::iterator it = nodeChildren.begin(); it != nodeChildren.end(); ++it)
  {
    tmpNodes.push_back( DeepFirstSearch( *it, out, er, connStubIO, tStubIO ) );
  }
  assert(tmpNodes.size() >= 2);
  NodeChildrenVector vec(tmpNodes[0], tmpNodes[1]);
  for(unsigned int i = 2; i<tmpNodes.size(); ++i)
  {
    vec.push_back(tmpNodes[i]);
  }
  GraphNodePtrNN graphNode(new GraphNode( er.readMetaAlert(id), connStubIO, tStubIO, vec ));
  out.push_back(graphNode);
  return graphNode;
}

void Restorer::Restore(EntryReader &er, NodesVector &out, vector<DataBaseID> &malerts)
{

  for(vector<DataBaseID>::iterator it = malerts.begin(); it != malerts.end(); ++it)
  {
    vector<DataBaseID> malertChildren( er.readMetaAlertChildren( (*it) ) );
    // put this data to the tree which represents meta alerts tree structure
    treeNodes_.insert( pair<Tree::IDNode, TreePtr>(*it, TreePtr(new Tree(*it, malertChildren) ) ) );
  }

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction tStubIO( connStubIO->createNewTransaction("stub transaction") );

  vector<DataBaseID> roots( er.readRoots());
  for(vector<DataBaseID>::iterator it = roots.begin(); it != roots.end(); ++it)
  {
    out.push_back( DeepFirstSearch(*it, out, er, connStubIO, tStubIO) );
  }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
