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
// TODO: test

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
  // TODO: use upper-case for type names
  /** \brief data type which stores tree nodes of class Tree
   */
  typedef std::map<DataBaseID, TreePtr> nodesMap;

  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);
  BackendFactory::FactoryPtr createStubIO(void);
  TreePtr getNode(DataBaseID id);
  GraphNodePtrNN deepFirstSearch(DataBaseID                                      id,
                                 NodesVector                                    &out,
                                 Persistency::IO::Postgres::detail::EntryReader &er,
                                 IO::ConnectionPtrNN                             connStubIO,
                                 IO::Transaction                                &tStubIO);
  // TODO: use private typedef for vector<ID> - it is used in few places already
  void restore(Persistency::IO::Postgres::detail::EntryReader &er,
               NodesVector                                    &out,
               std::vector<DataBaseID>                        &malerts);

  DBHandlerPtrNN  dbHandler_;
  nodesMap        treeNodes_;
}; // class Restorer

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
