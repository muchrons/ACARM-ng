/*
 * Restorer.cpp
 *
 */
#include <set>
#include <cassert>

#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/ExceptionBadNumberOfNodeChildren.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

using namespace Persistency::IO::Postgres::detail;
using namespace std;

namespace
{
template<typename T>
void removeDuplicates(std::vector<T> &out)
{
  std::set<T> s( out.begin(), out.end() );
  out.clear();
  out.assign( s.begin(), s.end() );
} // removeDuplicates()
} // unnamed namespace


namespace Persistency
{
namespace IO
{
namespace Postgres
{

Restorer::Restorer(Transaction   &t,
                   DBHandlePtrNN  dbHandle):
  IO::Restorer(t),
  log_("persistency.io.postgres.restorer"),
  dbHandle_(dbHandle)
{
}

void Restorer::restoreAllInUseImpl(Transaction &t, NodesVector &out)
{
  TRYCATCH_BEGIN
    EntryReader er(t, *dbHandle_);
    EntrySaver  es(t, *dbHandle_);
    const Tree::IDsVector &maInUse=er.readIDsMalertsInUse();
    const Tree::IDsVector &roots  =er.readRoots();
    Tree::IDsVector restoredIDs;
    restore(er, out, maInUse, roots, restoredIDs);
    markInvalidIDsAsUnused(es, maInUse, restoredIDs);
  TRYCATCH_END
}

void Restorer::restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to)
{
  TRYCATCH_BEGIN
    EntryReader er(t, *dbHandle_);
    const Tree::IDsVector  maBetween=er.readIDsMalertsBetween(from, to);
    const Tree::IDsVector &roots    =er.readRoots(from, to);
    Tree::IDsVector restoredIDs;
    restore(er, out, maBetween, roots, restoredIDs);
  TRYCATCH_END
}

BackendFactory::FactoryPtr Restorer::createStubIO(void)
{
  const BackendFactory::FactoryTypeName name("stubs");
  const BackendFactory::Options         options;
  return BackendFactory::create(name, options);
}


GraphNodePtrNN Restorer::makeLeaf(DataBaseID                  id,
                                  AlertPtrNN                  aPtr,
                                  Persistency::MetaAlert::ID  sysID,
                                  IO::ConnectionPtrNN         connStubIO,
                                  IO::Transaction            &tStubIO)
{
  if( leafCache_.has(id) )
    return leafCache_.get(id);
  GraphNodePtrNN leaf( new GraphNode( aPtr, sysID, connStubIO, tStubIO ) );
  leafCache_.add(id, leaf);
  return leaf;
}

GraphNodePtrNN Restorer::makeNode(DataBaseID                id,
                                  MetaAlertPtrNN            maPtr,
                                  const NodeChildrenVector &vec,
                                  IO::ConnectionPtrNN       connStubIO,
                                  IO::Transaction          &tStubIO)
{
  if( nodeCache_.has(id) )
    return nodeCache_.get(id);
  GraphNodePtrNN node( new GraphNode( maPtr, connStubIO, tStubIO, vec ) );
  nodeCache_.add(id, node);
  return node;
}

GraphNodePtrNN Restorer::deepFirstSearch(DataBaseID                                      id,
                                         NodesVector                                    &out,
                                         Persistency::IO::Postgres::detail::EntryReader &er,
                                         IO::ConnectionPtrNN                             connStubIO,
                                         IO::Transaction                                &tStubIO,
                                         Tree::IDsVector                                &restoredIDs)
{
  TreePtrNN node = treeNodes_.get(id);
  // check if there are no children (i.e. is leaf)
  if( node->getChildrenNumber() == 0 )
    return restoreLeaf(id, out, er, connStubIO, tStubIO);
  return restoreNode(node, id, out, er, connStubIO, tStubIO, restoredIDs);
}

void Restorer::restore(Persistency::IO::Postgres::detail::EntryReader &er,
                       NodesVector                                    &out,
                       const Tree::IDsVector                          &malerts,
                       const Tree::IDsVector                          &roots,
                       Tree::IDsVector                                &restoredIDs)
{
  addTreeNodesToCache(er, malerts);

  int p = 10;
  const size_t rootsCount = roots.size();

  IO::ConnectionPtrNN connStubIO( createStubIO() );
  IO::Transaction     tStubIO( connStubIO->createNewTransaction("stub_transaction") );

  for(Tree::IDsVector::const_iterator it = roots.begin(); it != roots.end(); ++it)
  {
    try
    {
      deepFirstSearch(*it, out, er, connStubIO, tStubIO, restoredIDs);
      restoredIDs.push_back(*it);
    }
    catch(const ExceptionNoSuchEntry &e)
    {
      LOGMSG_WARN_S(log_)<<"root with id "<< *it << " doesn't exist in cache; exception message: "<< e.what();
    }
    catch(const ExceptionBadNumberOfNodeChildren &e)
    {
      LOGMSG_WARN_S(log_) << "root with id " << *it
                          << " has bad number of children"
                             " this subtree is being skipped; exception message: "
                          << e.what();
    }
  }

  // though this is only a stub, it is better to commit it since automatic
  // rollback leaves ugly warning in log file.
  tStubIO.commit();

  // remove duplicates from out vector
  removeDuplicates(out);
  removeDuplicates(restoredIDs);
}

template<typename T>
void Restorer::addIfNew(const T &e, DataBaseID id)
{
  if(!dbHandle_->getIDCache()->has(e))
    dbHandle_->getIDCache()->add(e, id);
  else
    assert(id == dbHandle_->getIDCache()->get(e));
}

GraphNodePtrNN Restorer::restoreLeaf(DataBaseID                                      id,
                                     NodesVector                                    &out,
                                     Persistency::IO::Postgres::detail::EntryReader &er,
                                     IO::ConnectionPtrNN                             connStubIO,
                                     IO::Transaction                                &tStubIO)
{
  // read Alert from data base
  AlertPtrNN                       alertPtr( er.getLeaf(id) );
  const DataBaseID                 alertID = er.getAlertIDAssociatedWithMetaAlert(id);
  const Persistency::MetaAlert::ID sysID   = er.getSystemIDOfMetaAlert(id);
  // add Alert to cache
  addIfNew(alertPtr, alertID);
  const GraphNodePtrNN graphNodeLeaf( makeLeaf( alertID, alertPtr, sysID, connStubIO, tStubIO ) );
  // add Meta Alert to cache
  addIfNew(graphNodeLeaf->getMetaAlert(), id);
  out.push_back(graphNodeLeaf);
  return graphNodeLeaf;
}

GraphNodePtrNN Restorer::restoreNode(TreePtrNN                                       node,
                                     DataBaseID                                      id,
                                     NodesVector                                    &out,
                                     Persistency::IO::Postgres::detail::EntryReader &er,
                                     IO::ConnectionPtrNN                             connStubIO,
                                     IO::Transaction                                &tStubIO,
                                     Tree::IDsVector                                &restoredIDs)
{
  // read Meta Alert from data base
  MetaAlertPtrNN malertPtr( er.readMetaAlert(id) );
  // add Meta Alert to cache
  addIfNew(malertPtr, id);
  GraphNodePtrNN graphNode( makeNode( id,
                                      malertPtr,
                                      restoreNodeChildren(node, id, out, er, connStubIO, tStubIO, restoredIDs),
                                      connStubIO,
                                      tStubIO ) );
  out.push_back(graphNode);
  return graphNode;
}
NodeChildrenVector Restorer::restoreNodeChildren(TreePtrNN                                       node,
                                                 DataBaseID                                      id,
                                                 NodesVector                                    &out,
                                                 Persistency::IO::Postgres::detail::EntryReader &er,
                                                 IO::ConnectionPtrNN                             connStubIO,
                                                 IO::Transaction                                &tStubIO,
                                                 Tree::IDsVector                                &restoredIDs)
{
  vector<GraphNodePtrNN>  tmpNodes;
  // get children IDs vector for given node
  const Tree::IDsVector  &nodeChildren = node->getChildren();
  tmpNodes.reserve( nodeChildren.size() );
  // restore children
  for(Tree::IDsVector::const_iterator it = nodeChildren.begin();
      it != nodeChildren.end(); ++it)
  {
    try
    {
      tmpNodes.push_back( deepFirstSearch( *it, out, er, connStubIO, tStubIO, restoredIDs ) );
      restoredIDs.push_back(*it);
    }
    catch(const ExceptionNoSuchEntry &e)
    {
      LOGMSG_WARN_S(log_)<<"child with id "<< *it << " doesn't exist in cache; exception message: "<< e.what();
    }
    catch(const ExceptionBadNumberOfNodeChildren &e)
    {
      LOGMSG_WARN_S(log_)<<"child with id "<< *it << " has bad number of children; exception message: "<< e.what();
    }
  }
  if(tmpNodes.size() < 2)
    throw ExceptionBadNumberOfNodeChildren(SYSTEM_SAVE_LOCATION, id );
  // add first two children to the node children vector
  assert(tmpNodes.size() >= 2);
  NodeChildrenVector vec(tmpNodes[0], tmpNodes[1]);
  // add rest of children (indexing is started with 2 becouse node should have at least two children)
  for(size_t i = 2; i<tmpNodes.size(); ++i)
    vec.push_back(tmpNodes[i]);
  return vec;
}

void Restorer::addTreeNodesToCache(Persistency::IO::Postgres::detail::EntryReader &er,
                                   const Tree::IDsVector                          &malerts)
{
  int p = 10;
  const size_t malertsCount = malerts.size();
  std::map<DataBaseID, std::vector<DataBaseID> > malertsAllChildren = er.readAllMetaAlertsChildren();
  for(size_t i = 0; i < malertsCount; ++i)
  {
    const Tree::IDsVector &malertChildren = malertsAllChildren[malerts[i]];
    // put this data to the tree which represents meta alerts tree structure
    treeNodes_.add(malerts[i], TreePtr(new Tree(malerts[i], malertChildren) ));
    if( ((double)(i+1)/malertsCount) > 0.01*p)
    {
      LOGMSG_INFO_S(log_)<< "Reading meta-alerts tree - " << p << " % " << "completed";
      p+=10;
    }
  }
  LOGMSG_INFO_S(log_)<< "Reading meta-alerts tree - " << p << " % " << "completed";
}

void Restorer::markInvalidIDsAsUnused(Persistency::IO::Postgres::detail::EntrySaver  &es,
                                      Tree::IDsVector                                maInUse,
                                      Tree::IDsVector                                restoredIDs)
{
  removeDuplicates(maInUse);
  // sort is needed for set_difference method
  sort(maInUse.begin(), maInUse.end());
  sort(restoredIDs.begin(), restoredIDs.end());

  assert(maInUse.size() >= restoredIDs.size());
  const size_t size = maInUse.size() - restoredIDs.size();
  Tree::IDsVector removed( size );
  // invalid meta alerts set is difference of meta alerts in use set and restored meta alerts set
  Tree::IDsVector::iterator end = set_difference( maInUse.begin(),     maInUse.end(),
                                                  restoredIDs.begin(), restoredIDs.end(),
                                                  removed.begin() );
  assert( size == removed.size() );
  // mark invalid meta alerts as unused
  for(Tree::IDsVector::iterator it = removed.begin(); it != end; ++it)
    es.markMetaAlertAsUnused(*it);
}
} // namespace Postgres
} // namespace IO
} // namespace Persistency
