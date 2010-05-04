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
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"

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
  // TODO: make template class to hold map<ID,T> inside and provided
  //       proper API. this will make this class much shorter and
  //       more readable. remember "single responsibility rule", i.e.
  //       one-class -> one-responsibility.
  //       btw: there was funny picture about this rule:
  //       http://blackbeltreview.files.wordpress.com/2009/12/singleresponsibilityprinciple2_71060858.jpg
  typedef std::map<DataBaseID, TreePtr>        NodesMap;
  typedef std::map<DataBaseID, GraphNodePtrNN> GraphNodesMap;

  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);

  BackendFactory::FactoryPtr createStubIO(void);
  // TODO: this method should be const
  TreePtr getNode(DataBaseID id);
  // TODO: this method should be const
  bool isInCache(DataBaseID id);
  // TODO: this method should be const
  GraphNodePtrNN getFromCache(DataBaseID id);
  void addToCache(DataBaseID id, GraphNodePtrNN node);

  // TODO: this method should be const
  // TODO: proper name for this methods is makeLeaf()
  GraphNodePtrNN getNode(DataBaseID          id,
                         AlertPtrNN          aPtr,
                         IO::ConnectionPtrNN connStubIO,
                         IO::Transaction     &tStubIO);
  // TODO: this method should be const
  // TODO: proper name for this methods is makeNode()
  GraphNodePtrNN getNode(DataBaseID          id,
                         MetaAlertPtrNN      maPtr,
                         NodeChildrenVector  &vec,
                         IO::ConnectionPtrNN connStubIO,
                         IO::Transaction     &tStubIO);
  GraphNodePtrNN deepFirstSearch(DataBaseID                                      id,
                                 NodesVector                                    &out,
                                 Persistency::IO::Postgres::detail::EntryReader &er,
                                 IO::ConnectionPtrNN                             connStubIO,
                                 IO::Transaction                                &tStubIO);
  void restore(Persistency::IO::Postgres::detail::EntryReader &er,
               NodesVector                                    &out,
               Tree::IDsVector                                &malerts);

  template<typename T>
  void addIfNew(T e, DataBaseID id);

  DBHandlerPtrNN  dbHandler_;
  GraphNodesMap   graphCache_;
  NodesMap        treeNodes_;
}; // class Restorer

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
