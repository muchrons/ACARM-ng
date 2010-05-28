/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE

#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/Postgres/Tree.hpp"
#include "Persistency/IO/Postgres/ReverseIDCache.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief restore data base operations.
 */
class Restorer: public IO::Restorer
{
public:
  /** \brief create restore from persistency proxy object
   *  \param t         active transaction.
   *  \param dbHandler shared connection to data base.
   */
  Restorer(Transaction    &t,
           DBHandlerPtrNN  dbHandler);

private:
  /** \brief data type which stores tree nodes of class Tree
   */

  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);

  BackendFactory::FactoryPtr createStubIO(void);

  GraphNodePtrNN makeLeaf(DataBaseID          id,
                          AlertPtrNN          aPtr,
                          IO::ConnectionPtrNN connStubIO,
                          IO::Transaction     &tStubIO);
  GraphNodePtrNN makeNode(DataBaseID                id,
                          MetaAlertPtrNN            maPtr,
                          const NodeChildrenVector &vec,
                          IO::ConnectionPtrNN       connStubIO,
                          IO::Transaction          &tStubIO);
  GraphNodePtrNN deepFirstSearch(DataBaseID                                      id,
                                 NodesVector                                    &out,
                                 Persistency::IO::Postgres::detail::EntryReader &er,
                                 IO::ConnectionPtrNN                             connStubIO,
                                 IO::Transaction                                &tStubIO,
                                 Tree::IDsVector                                &restoredIDs);

  void restore(Persistency::IO::Postgres::detail::EntryReader &er,
               NodesVector                                    &out,
               const Tree::IDsVector                          &malerts,
               const Tree::IDsVector                          &roots,
               Tree::IDsVector                                &restoredIDs);

  template<typename T>
  void addIfNew(const T &e, DataBaseID id);

  GraphNodePtrNN restoreLeaf(DataBaseID                                      id,
                             NodesVector                                    &out,
                             Persistency::IO::Postgres::detail::EntryReader &er,
                             IO::ConnectionPtrNN                             connStubIO,
                             IO::Transaction                                &tStubIO);

  GraphNodePtrNN restoreNode(TreePtrNN                                       node,
                             DataBaseID                                      id,
                             NodesVector                                    &out,
                             Persistency::IO::Postgres::detail::EntryReader &er,
                             IO::ConnectionPtrNN                             connStubIO,
                             IO::Transaction                                &tStubIO,
                             Tree::IDsVector                                &restoredIDs);

  NodeChildrenVector restoreNodeChildren(TreePtrNN                                       node,
                                         DataBaseID                                      id,
                                         NodesVector                                    &out,
                                         Persistency::IO::Postgres::detail::EntryReader &er,
                                         IO::ConnectionPtrNN                             connStubIO,
                                         IO::Transaction                                &tStubIO,
                                         Tree::IDsVector                                &restoredIDs);

  void addTreeNodesToCache(Persistency::IO::Postgres::detail::EntryReader &er,
                           const Tree::IDsVector                          &malerts);

  void markInvalidIDsAsUnused(Persistency::IO::Postgres::detail::EntrySaver  &es,
                              Tree::IDsVector                                maInUse,
                              Tree::IDsVector                                restoredIDs);


  Logger::Node                   log_;
  DBHandlerPtrNN                 dbHandler_;
  ReverseIDCache<GraphNodePtrNN> nodeCache_;
  ReverseIDCache<GraphNodePtrNN> leafCache_;
  ReverseIDCache<TreePtr>        treeNodes_;
}; // class Restorer

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
