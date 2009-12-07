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
#include "Persistency/IO/Graph.hpp"
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
  virtual ~Connection(void);

  TransactionAPIAutoPtr createNewTransaction(const std::string &name);
  AlertAutoPtr alert(AlertPtr alert, const Transaction &t);
  GraphAutoPtr graph(const Transaction &t);
  HostAutoPtr host(HostPtr host, const Transaction &t);
  MetaAlertAutoPtr metaAlert(MetaAlertPtr, const Transaction &t);

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(const std::string &name) = 0;
  virtual AlertAutoPtr alertImpl(AlertPtr alert, const Transaction &t) = 0;
  virtual GraphAutoPtr graphImpl(const Transaction &t) = 0;
  virtual HostAutoPtr hostImpl(HostPtr host, const Transaction &t) = 0;
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtr, const Transaction &t) = 0;

  Base::Threads::Mutex mutex_;
}; // class Connection


/** \brief smart pointer to this type.
 */
typedef boost::shared_ptr<Connection> ConnectionPtr;

} // namespace IO
} // namespace Persistency

#endif
