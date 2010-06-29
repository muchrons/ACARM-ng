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
         typename TTransactionAPIIO,
         typename TAlertIO,
         typename THostIO,
         typename TMetaAlertIO,
         typename TDynamicConfig,
         typename TRestorerIO>
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
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name)
  {
    return TransactionAPIAutoPtr( new TTransactionAPIIO(mutex, name, ph_) );
  }

  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, Transaction &t)
  {
    return AlertAutoPtr( new TAlertIO(alert, t, ph_) );
  }

  virtual HostAutoPtr hostImpl(HostPtrNN host, Transaction &t)
  {
    return HostAutoPtr( new THostIO(host, t, ph_) );
  }

  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, Transaction &t)
  {
    return MetaAlertAutoPtr( new TMetaAlertIO(ma, t, ph_) );
  }

  virtual DynamicConfigAutoPtr dynamicConfigImpl(const DynamicConfig::Owner &owner, Transaction &t)
  {
    return DynamicConfigAutoPtr( new TDynamicConfig(owner, t) );
  }

  virtual RestorerAutoPtr restorerImpl(Transaction &t)
  {
    return RestorerAutoPtr( new TRestorerIO(t, ph_) );
  }

  // there is no generic implementation for this call - it has to be
  // implemented separately for each backend.
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t) = 0;

  TPersistencyHandler ph_;
}; // class ConnectionHelper

} // namespace IO
} // namespace Persistency

#endif
