/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Host.hpp"
#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/TransactionAPI.hpp"

namespace Persistency
{
namespace IO
{

/** \brief Connection to the persistency API
 */
class Connection: private boost::noncopyable
{
public:
  /** \brief virtual d-tor for plymorphic base class.
   */
  virtual ~Connection(void);

  /** \brief creates new transaction, of a given name.
   *  \param name name for new transaction.
   *  \return transaction API object for given persistency type.
   *  \note opening transaction locks communication object so that only
   *        one transaction can be created at a time. if more call will
   *        appear, they will be waiting for it to finish.
   */
  TransactionAPIAutoPtr createNewTransaction(const std::string &name);
  /** \brief create alert's persistency proxy.
   *  \param alert alert to work on.
   *  \param t     active transaction.
   *  \return alert's persistency proxy object.
   */
  AlertAutoPtr alert(AlertPtr alert, const Transaction &t);
  /** \brief create host's persistency proxy.
   *  \param host host to work on.
   *  \param t    active transaction.
   *  \return host's persistency proxy.
   */
  HostAutoPtr host(HostPtr host, const Transaction &t);
  /** \brief creates meta-alert proxy object.
   *  \param ma meta alert to work on.
   *  \param t  active transaction.
   *  \return meta-alert persistency proxy.
   */
  MetaAlertAutoPtr metaAlert(MetaAlertPtr ma, const Transaction &t);

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name) = 0;
  virtual AlertAutoPtr alertImpl(AlertPtr alert, const Transaction &t) = 0;
  virtual HostAutoPtr hostImpl(HostPtr host, const Transaction &t) = 0;
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtr ma, const Transaction &t) = 0;

  Base::Threads::Mutex mutex_;
}; // class Connection


/** \brief smart pointer to this type.
 */
typedef boost::shared_ptr<Connection> ConnectionPtr;

} // namespace IO
} // namespace Persistency

#endif
