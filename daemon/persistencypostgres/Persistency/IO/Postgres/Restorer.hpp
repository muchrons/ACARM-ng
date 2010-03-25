/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE

#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Postgres/Tree.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/GraphNode.hpp"
// TODO: test

using namespace Persistency::IO::Postgres::detail;
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
  typedef std::map<Tree::IDNode, TreePtr> nodesMap;
  /** \brief create restore from persistency proxy object
   *  \param t         active transaction.
   *  \param dbHandler shared connection to data base.
   */
  Restorer(Transaction    &t,
           DBHandlerPtrNN  dbHandler);

private:
  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);
  BackendFactory::FactoryPtr createStubIO(void);
  TreePtr getNode(Tree::IDNode id );
  int getNumberOfChildren(Tree::IDNode id );
  GraphNodePtrNN DeepFirstSearch(Tree::IDNode         id,
                                 NodesVector         &out,
                                 EntryReader         &er,
                                 IO::ConnectionPtrNN  connStubIO,
                                 IO::Transaction     &tStubIO);

  DBHandlerPtrNN  dbHandler_;
  nodesMap        treeNodes_;

  void Restore(EntryReader &er, NodesVector &out, vector<DataBaseID> &malerts);
}; // class Restorer

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
