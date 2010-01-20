/*
 * BackendProxy.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_BACKENDPROXY_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_BACKENDPROXY_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/IO/Connection.hpp"


namespace Core
{
namespace Types
{
namespace Proc
{
/** \brief persistent storage communication simplification layer.
 *
 *  common parts of all backend proxy implementation are gathered here.
 *
 *  \note this object allows transaction's to take place, but only on persistent
 *        level, i.e. rollbacking transaction does not change user objects.
 */
class BackendProxy: private boost::noncopyable
{
public:
  /** \brief deallocates object's internal resources.
   */
  virtual ~BackendProxy(void);
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
   *  \param conn          connection object to use.
   *  \param processorName name of processor this object is created for.
   */
  BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
               const std::string                &processorName);
  /** \brief ensure transaction is in progress (by running new, if needed).
   *  \note if transaciton is started, call does nothing. otherwise starts
   *        new one.
   */
  void beginTransaction(void);
  /** \brief gets transaction object.
   *  \return ongoing transaction.
   */
  Persistency::IO::Transaction &getTransaction(void) const;

private:
  typedef boost::scoped_ptr<Persistency::IO::Transaction> TransactionScPtr;

  std::string                      processorName_;
  Persistency::IO::ConnectionPtrNN conn_;
  TransactionScPtr                 transaction_;
}; // class BackendProxy

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
