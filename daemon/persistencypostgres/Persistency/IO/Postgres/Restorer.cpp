/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/ExceptionBadNumberOfNodeChildren.hpp"
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
  Tree::IDsVector maInUse( er.readIDsMalertsInUse() );
  restore(er, out, maInUse);
}

void Restorer::restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to)
{
  EntryReader er(t, *dbHandler_);
  Tree::IDsVector maBetween( er.readIDsMalertsBetween(from, to) );
  restore(er, out, maBetween);
}

BackendFactory::FactoryPtr Restorer::createStubIO(void)
{
  const BackendFactory::FactoryTypeName name("stubs");
  const BackendFactory::Options         options;
  return BackendFactory::create(name, options);
}


TreePtr Restorer::getNode(DataBaseID id )
{
  if(treeNodes_.count(id) > 0)
    return treeNodes_.find(id)->second;
  else
    return TreePtr();
}

GraphNodePtrNN Restorer::deepFirstSearch(DataBaseID                                      id,
                                         NodesVector                                    &out,
                                         Persistency::IO::Postgres::detail::EntryReader &er,
                                         IO::ConnectionPtrNN                             connStubIO,
                                         IO::Transaction                                &tStubIO)
{
  TreePtrNN node = getNode(id);
  // check if there are no children (i.e. is leaf)
  if( node->getChildrenNumber() == 0 )
  {
    // read Alert from data base
    AlertPtrNN alertPtr( er.getLeaf(id) );
    const DataBaseID alertID = er.getAlertIDAssociatedWithMetaAlert(id);
    // add Alert to cache
    addIfNew(alertPtr, alertID);
    return GraphNodePtrNN( new GraphNode( alertPtr, connStubIO, tStubIO ) );
  }
  vector<GraphNodePtrNN>  tmpNodes;
  const Tree::IDsVector  &nodeChildren = node->getChildren();
  tmpNodes.reserve( nodeChildren.size() );
  for(Tree::IDsVector::const_iterator it = nodeChildren.begin();
      it != nodeChildren.end(); ++it)
  {
    tmpNodes.push_back( deepFirstSearch( *it, out, er, connStubIO, tStubIO ) );
  }
  if(tmpNodes.size() < 2)
    throw ExceptionBadNumberOfNodeChildren(SYSTEM_SAVE_LOCATION, id );
  NodeChildrenVector vec(tmpNodes[0], tmpNodes[1]);
  for(size_t i = 2; i<tmpNodes.size(); ++i)
    vec.push_back(tmpNodes[i]);
  // read Meta Alert from data base
  MetaAlertPtrNN malertPtr( er.readMetaAlert(id) );
  // add Meta Alert to cache
  addIfNew(malertPtr, id);
  GraphNodePtrNN graphNode(new GraphNode( malertPtr,
                                          connStubIO, tStubIO, vec ));
  out.push_back(graphNode);
  return graphNode;
}

void Restorer::restore(Persistency::IO::Postgres::detail::EntryReader &er,
                       NodesVector                                    &out,
                       Tree::IDsVector                                &malerts)
{
  for(Tree::IDsVector::const_iterator it = malerts.begin(); it != malerts.end(); ++it)
  {
    const Tree::IDsVector &malertChildren = er.readMetaAlertChildren( (*it) );
    // put this data to the tree which represents meta alerts tree structure
    pair<DataBaseID, TreePtr> tmp(*it, TreePtr(new Tree(*it, malertChildren) ) );
    treeNodes_.insert(tmp);
  }

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction     tStubIO( connStubIO->createNewTransaction("stub transaction") );

  const Tree::IDsVector &roots = er.readRoots();
  for(Tree::IDsVector::const_iterator it = roots.begin(); it != roots.end(); ++it)
    out.push_back(deepFirstSearch(*it, out, er, connStubIO, tStubIO));
  //TODO: try - catch
}

template<typename T>
void Restorer::addIfNew(T e, DataBaseID id)
{
  if(!dbHandler_->getIDCache()->has(e))
    dbHandler_->getIDCache()->add(e, id);
  else
    assert(id == dbHandler_->getIDCache()->get(e));
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
