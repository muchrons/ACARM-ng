/*
 * Restorer.cpp
 *
 */
#include <set>
// TODO: cassert is not included, but assert() is used

#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/ExceptionBadNumberOfNodeChildren.hpp"

using namespace Persistency::IO::Postgres::detail;
using namespace std;

namespace
{
template<typename T>
void removeDuplicates(std::vector<T> &out)
{
  std::set<T> s( out.begin(), out.end() );
  out.erase( out.begin(), out.end() );
  out.assign( s.begin(), s.end() );
} // removeDuplicates()
} // unnamed namespace


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
  restore(er, out, maBetween, from, to);
}

BackendFactory::FactoryPtr Restorer::createStubIO(void)
{
  const BackendFactory::FactoryTypeName name("stubs");
  const BackendFactory::Options         options;
  return BackendFactory::create(name, options);
}


GraphNodePtrNN Restorer::makeLeaf(DataBaseID           id,
                                 AlertPtrNN           aPtr,
                                 IO::ConnectionPtrNN  connStubIO,
                                 IO::Transaction     &tStubIO)
{
  if( graphCache_.isInCache(id) )
    return graphCache_.getFromCacheNotNull(id);
  GraphNodePtrNN leaf( new GraphNode( aPtr, connStubIO, tStubIO ) );
  graphCache_.addToCache(id, leaf);
  return leaf;
}

GraphNodePtrNN Restorer::makeNode(DataBaseID           id,
                                  MetaAlertPtrNN       maPtr,
                                  NodeChildrenVector   vec,          // TODO: const-ref here
                                  IO::ConnectionPtrNN  connStubIO,
                                  IO::Transaction     &tStubIO)
{
  if( graphCache_.isInCache(id) )
    return graphCache_.getFromCacheNotNull(id);
  GraphNodePtrNN node( new GraphNode( maPtr, connStubIO, tStubIO, vec ) );
  graphCache_.addToCache(id, node);
  return node;
}

GraphNodePtrNN Restorer::deepFirstSearch(DataBaseID                                      id,
                                         NodesVector                                    &out,
                                         Persistency::IO::Postgres::detail::EntryReader &er,
                                         IO::ConnectionPtrNN                             connStubIO,
                                         IO::Transaction                                &tStubIO)
{
  TreePtrNN node = treeNodes_.getFromCache(id);
  // check if there are no children (i.e. is leaf)
  if( node->getChildrenNumber() == 0 )
    return restoreLeaf(id, out, er, connStubIO, tStubIO);
  // TODO: id' suggest making below code resotreNode() method, for consistency and readability
  // read Meta Alert from data base
  MetaAlertPtrNN malertPtr( er.readMetaAlert(id) );
  // add Meta Alert to cache
  addIfNew(malertPtr, id);
  GraphNodePtrNN graphNode( makeNode( id, malertPtr,
                                         restoreNodeChildren(node, id, out, er, connStubIO, tStubIO),
                                         connStubIO,
                                         tStubIO ));
  out.push_back(graphNode);
  return graphNode;
}

// TODO: add class with lists of errors
void Restorer::restore(Persistency::IO::Postgres::detail::EntryReader &er,
                       NodesVector                                    &out,
                       Tree::IDsVector                                &malerts)
{
  // TODO: this method's implemenation is identical with restore(..., from, to).
  //       make this common code, that takes Tree::IDsVector as a parameter - this
  //       will give you single implementation with two one-line API calls.
  addTreeNodesToCache(er, malerts);

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction     tStubIO( connStubIO->createNewTransaction("stub transaction") );

  const Tree::IDsVector &roots = er.readRoots();
  for(Tree::IDsVector::const_iterator it = roots.begin(); it != roots.end(); ++it)
  {
    try
    {
      deepFirstSearch(*it, out, er, connStubIO, tStubIO);
    }
    catch(const ExceptionBadNumberOfNodeChildren &)
    {
      // TODO: logs
    }
  }
  // remove doplicates from out vector
  removeDuplicates(out);
}

// TODO: reorganize this code
void Restorer::restore(Persistency::IO::Postgres::detail::EntryReader &er,
                       NodesVector                                    &out,
                       Tree::IDsVector                                &malerts,
                       const Timestamp                                &from,
                       const Timestamp                                &to)
{
  // TODO: see comment to restore(..., from, to)
  addTreeNodesToCache(er, malerts);

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction     tStubIO( connStubIO->createNewTransaction("stub transaction") );

  const Tree::IDsVector &roots = er.readRoots(from, to);
  for(Tree::IDsVector::const_iterator it = roots.begin(); it != roots.end(); ++it)
  {
    try
    {
      deepFirstSearch(*it, out, er, connStubIO, tStubIO);
    }
    catch(const ExceptionBadNumberOfNodeChildren &)
    {
      // TODO: logs
    }
  }
  // remove doplicates from out vector
  removeDuplicates(out);
}


// TODO: take T as const-reference - it can be gib object after all
// TODO: consider returing element from cache, if addition has not been made or
//       'e' when new one has been added - it's trivial to implement and makes
//       API more robust.
template<typename T>
void Restorer::addIfNew(T e, DataBaseID id)
{
  if(!dbHandler_->getIDCache()->has(e))
    dbHandler_->getIDCache()->add(e, id);
  else
    assert(id == dbHandler_->getIDCache()->get(e));
}

GraphNodePtrNN Restorer::restoreLeaf(DataBaseID                                      id,
                                     NodesVector                                    &out,
                                     Persistency::IO::Postgres::detail::EntryReader &er,
                                     IO::ConnectionPtrNN                             connStubIO,
                                     IO::Transaction                                &tStubIO)
{
  // TODO: fix indentation
    // read Alert from data base
    AlertPtrNN alertPtr( er.getLeaf(id) );
    const DataBaseID alertID = er.getAlertIDAssociatedWithMetaAlert(id);
    // add Alert to cache
   addIfNew(alertPtr, alertID);
  // TODO: creating new graph node/leaf, when alert was already processed is not valid since
  //       it makes double instances of object repreesnting the same elements.
  //       graph nodes probably should be cached as well, indexed by alerts' ID and nodes' ID.
    const GraphNodePtrNN graphNodeLeaf( makeLeaf( alertID, alertPtr, connStubIO, tStubIO ) );
    out.push_back(graphNodeLeaf);
    return graphNodeLeaf;
}

NodeChildrenVector Restorer::restoreNodeChildren(TreePtrNN                                       node,
                                                 DataBaseID                                      id,
                                                 NodesVector                                    &out,
                                                 Persistency::IO::Postgres::detail::EntryReader &er,
                                                 IO::ConnectionPtrNN                             connStubIO,
                                                 IO::Transaction                                &tStubIO)
{
  // TODO: comment 3 main parts/blocks of this method
  vector<GraphNodePtrNN>  tmpNodes;
  const Tree::IDsVector  &nodeChildren = node->getChildren();
  if(nodeChildren.size() < 2)
    throw ExceptionBadNumberOfNodeChildren(SYSTEM_SAVE_LOCATION, id );
  tmpNodes.reserve( nodeChildren.size() );
  for(Tree::IDsVector::const_iterator it = nodeChildren.begin();
      it != nodeChildren.end(); ++it)
  {
    tmpNodes.push_back( deepFirstSearch( *it, out, er, connStubIO, tStubIO ) );
  }
  // TODO: assert on tmpNodes' size would be nice here
  NodeChildrenVector vec(tmpNodes[0], tmpNodes[1]);
  // TODO: comment this code a bit, i.e. why indexing is started with 2
  for(size_t i = 2; i<tmpNodes.size(); ++i)
    vec.push_back(tmpNodes[i]);
  return vec;
}

// TODO: 'malerts' is not changed here - it should be const-ref.
// TODO: can 'er' be const-ref too?
void Restorer::addTreeNodesToCache(Persistency::IO::Postgres::detail::EntryReader &er, Tree::IDsVector &malerts)
{
  for(Tree::IDsVector::const_iterator it = malerts.begin(); it != malerts.end(); ++it)
  {
    const Tree::IDsVector &malertChildren = er.readMetaAlertChildren( (*it) );
    // put this data to the tree which represents meta alerts tree structure
    treeNodes_.addToCache(*it, TreePtr(new Tree(*it, malertChildren) ));
  }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
