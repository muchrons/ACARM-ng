/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Host.hpp"
#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/TransactionAPI.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Persistency/IO/Restorer.hpp"
#include "Persistency/IO/Heartbeats.hpp"
#include "Persistency/IO/Exception.hpp"

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
   *  \return non-NULL transaction API object for given persistency type.
   *  \note opening transaction locks communication object so that only
   *        one transaction can be created at a time. if more calls will
   *        appear, they will be waiting for it to finish.
   */
  TransactionAPIAutoPtr createNewTransaction(const std::string &name);

  /** \brief create alert's persistency proxy.
   *  \param alert alert to work on.
   *  \param t     active transaction.
   *  \return non-NULL alert's persistency proxy object.
   */
  AlertAutoPtr alert(AlertPtrNN alert, Transaction &t);

  /** \brief create host's persistency proxy.
   *  \param host host to work on.
   *  \param t    active transaction.
   *  \return non-NULL host's persistency proxy.
   */
  HostAutoPtr host(HostPtrNN host, Transaction &t);

  /** \brief creates meta-alert proxy object.
   *  \param ma meta alert to work on.
   *  \param t  active transaction.
   *  \return non-NULL meta-alert persistency proxy.
   */
  MetaAlertAutoPtr metaAlert(MetaAlertPtrNN ma, Transaction &t);

  /** \brief create meta-alert persistency proxy.
   *  \param owner owner's name of a configuration to work on (NULL means common config).
   *  \param t     active transaction.
   *  \return return non-NULL dynamic configuration proxy.
   */
  DynamicConfigAutoPtr dynamicConfig(const DynamicConfig::Owner &owner, Transaction &t);

  /** \brief create restorer object.
   *  \param t transaction to be used for restoring data.
   *  \return non-NULL restorer persistency proxy.
   */
  RestorerAutoPtr restorer(Transaction &t);

  /** \brief clean up saved entries older given number of days.
   *  \param days maximum age of entry, counted in days.
   *  \param t    transaction to be used when performing clean-up.
   *  \return number of alerts removed from persistent storage.
   *  \note entries that are still used by the system are not removed,
   *        event if they are aready too old.
   */
  size_t removeEntriesOlderThan(size_t days, Transaction &t);

  /** \brief run periodic actions on persistent storage engine
   *  param t  transaction to be used when performing clean-up.
   */
  void issuePeriodicQueries(Transaction &t);

  /** \brief create heartbeats reporting object.
   *  \param owner subsystem, that will send heartbeats.
   *  \param t     transaction to be used for reporting data.
   *  \return non-NULL heartbeats reporting proxy.
   */
  HeartbeatsAutoPtr heartbeats(const Heartbeats::Owner &owner, Transaction &t);

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name) = 0;
  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, Transaction &t) = 0;
  virtual HostAutoPtr hostImpl(HostPtrNN host, Transaction &t) = 0;
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, Transaction &t) = 0;
  virtual DynamicConfigAutoPtr dynamicConfigImpl(const DynamicConfig::Owner &owner,
                                                 Transaction                &t) = 0;
  virtual RestorerAutoPtr restorerImpl(Transaction &t) = 0;
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t) = 0;
  virtual void issuePeriodicSystemQueriesImpl(Transaction &t) = 0;
  virtual void issuePeriodicUserQueriesImpl(Transaction &t) = 0;
  virtual HeartbeatsAutoPtr heartbeatsImpl(const Heartbeats::Owner &owner, Transaction &t) = 0;

  // mutex is used to ensure only one transaction can be created at a time.
  Base::Threads::Mutex mutex_;
}; // class Connection

/** \brief smart pointer to this type, checked not to be NULL. */
typedef Commons::SharedPtrNotNULL<Connection> ConnectionPtrNN;

} // namespace IO
} // namespace Persistency

#endif
