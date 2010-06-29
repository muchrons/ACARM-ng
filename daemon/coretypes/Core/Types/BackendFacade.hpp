/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_BACKENDFACADE_HPP_FILE
#define INCLUDE_CORE_TYPES_BACKENDFACADE_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/IO/Connection.hpp"


namespace Core
{
namespace Types
{
/** \brief persistent storage communication simplification layer.
 *
 *  common parts of all backend proxy implementation are gathered here.
 *
 *  \note this object allows transaction's to take place, but only on persistent
 *        level, i.e. rollbacking transaction does not change user objects.
 */
class BackendFacade: private boost::noncopyable
{
public:
  /** \brief deallocates object's internal resources.
   */
  virtual ~BackendFacade(void);
  /** \brief commit current transaction.
   *
   *  if not transaction is started, call does nothing. if transaction is opened,
   *  it is commited.
   *
   *  \note calling another call after commitChanges will open new transaction.
   */
  void commitChanges(void);

protected:
  /** \brief create object's instance.
   *  \param conn connection object to use.
   *  \param name name of element this object is created for (i.e.: filter/trigger/input name).
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                const std::string                &name);
  /** \brief ensure transaction is in progress (by running new, if needed).
   *  \note if transaciton is started, call does nothing. otherwise starts
   *        new one.
   */
  void beginTransaction(void);
  /** \brief gets transaction object.
   *  \return ongoing transaction.
   */
  Persistency::IO::Transaction &getTransaction(void);
  /** \brief gets connection object.
   *  \return connection object.
   */
  Persistency::IO::ConnectionPtrNN getConnection(void);
  /** \brief gets name of this element.
   *  \return elements name.
   */
  const std::string &getName(void) const;

private:
  typedef boost::scoped_ptr<Persistency::IO::Transaction> TransactionScPtr;

  std::string                      name_;
  Persistency::IO::ConnectionPtrNN conn_;
  TransactionScPtr                 transaction_;
}; // class BackendFacade

} // namespace Types
} // namespace Core

#endif
