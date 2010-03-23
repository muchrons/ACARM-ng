/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_RESTORER_HPP_FILE

//#include "Persistency/IO/Restorer.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Connection.hpp"
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
  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);
  BackendFactory::FactoryPtr createStubIO(void);
  /*
  work in progress
  IO::ConnectionPtrNN  connectionStubIO_;
  typedef std::map<DataBaseID, GraphNodePtrNN> leafsMap;
  */
  DBHandlerPtrNN  dbHandler_;
}; // class Restorer

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
