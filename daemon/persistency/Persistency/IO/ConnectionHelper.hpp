/*
 * ConnectionHelper.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_CONNECTIONHELPER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_CONNECTIONHELPER_HPP_FILE

/* public header */

#include "Persistency/IO/Connection.hpp"

namespace Persistency
{
namespace IO
{

/** \brief Helper class for easier creating basic implementations of derived ones.
 */
template<typename TPersistencyHandler,
         typename TTransactionIO,
         typename TAlertIO,
         typename THostIO,
         typename TMetaAlertIO>
class ConnectionHelper: public Connection
{
public:
  /** \brief create connection helper with a given persistency handler.
   *
   *  creates persistency connection creator, with given arguments that
   *  will be passed to each execution of transaction creation.
   *
   *  \param ph user-defined persistency handler.
   */
  explicit ConnectionHelper(const TPersistencyHandler &ph):
    ph_(ph)
  {
  }

private:
  virtual TransactionAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name)
  {
    return TransactionAutoPtr( new TTransactionIO(mutex, name, ph_) );
  }

  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, const Transaction &t)
  {
    return AlertAutoPtr( new TAlertIO(alert, t, ph_) );
  }

  virtual HostAutoPtr hostImpl(HostPtrNN host, const Transaction &t)
  {
    return HostAutoPtr( new THostIO(host, t, ph_) );
  }

  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, const Transaction &t)
  {
    return MetaAlertAutoPtr( new TMetaAlertIO(ma, t, ph_) );
  }

  TPersistencyHandler ph_;
}; // class ConnectionHelper

} // namespace IO
} // namespace Persistency

#endif
