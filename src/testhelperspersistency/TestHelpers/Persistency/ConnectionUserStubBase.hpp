/*
 * ConnectionUserStubBase.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONUSERSTUBBASE_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONUSERSTUBBASE_HPP_FILE

/* public header */

#include "Persistency/IO/Connection.hpp"

namespace TestHelpers
{
namespace Persistency
{

/** \brief helper class for creating more specialized IO objects for tests.
 */
class ConnectionUserStubBase: public ::Persistency::IO::Connection
{
public:
  /** \brief create generic class that provides default stubs for all calls and allows
   *         easy overwriting specific methods by user.
   */
  ConnectionUserStubBase(void);

protected:
  // genericStub_ accessor
  ::Persistency::IO::ConnectionPtrNN getStub(void);

private:
  virtual ::Persistency::IO::TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                                            const std::string    &name);
  virtual ::Persistency::IO::AlertAutoPtr alertImpl(::Persistency::AlertPtrNN alert, ::Persistency::IO::Transaction &t);
  virtual ::Persistency::IO::HostAutoPtr hostImpl(::Persistency::HostPtrNN host, ::Persistency::IO::Transaction &t);
  virtual ::Persistency::IO::MetaAlertAutoPtr metaAlertImpl(::Persistency::MetaAlertPtrNN ma, ::Persistency::IO::Transaction &t);
  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t);
  virtual ::Persistency::IO::RestorerAutoPtr restorerImpl(::Persistency::IO::Transaction &t);
  virtual size_t removeEntriesOlderThanImpl(size_t days, ::Persistency::IO::Transaction &t);
  virtual void issuePeriodicSystemQueriesImpl(::Persistency::IO::Transaction &t);
  virtual void issuePeriodicUserQueriesImpl(::Persistency::IO::Transaction &t);
  virtual ::Persistency::IO::HeartbeatsAutoPtr heartbeatsImpl(const ::Persistency::IO::Heartbeats::Owner &owner,
                                                              ::Persistency::IO::Transaction             &t);

  ::Persistency::IO::ConnectionPtrNN genericStub_;
}; // class ConnectionUserStubBase

} // namespace Persistency
} // namespace TestHelpers

#endif
