/*
 * IOMetaAlertStub.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_IOMETAALERTSTUB_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_IOMETAALERTSTUB_HPP_FILE

/* public header */

#include "Persistency/IO/MetaAlert.hpp"

namespace TestHelpers
{
namespace Persistency
{

/** \brief stub for operations on meta-alert
 */
class IOMetaAlertStub: public ::Persistency::IO::MetaAlert
{
public:
  /** \brief meta alert persistency IO stub.
   *  \param ma     meta alert to work on.
   *  \param t      active transaction.
   */
  IOMetaAlertStub(::Persistency::MetaAlertPtrNN   ma,
                  ::Persistency::IO::Transaction &t):
    MetaAlert(ma, t)
  {
  }

private:
  virtual void saveImpl(::Persistency::IO::Transaction &/*t*/)
  {
  }

  virtual void markAsTriggeredImpl(::Persistency::IO::Transaction &/*t*/, const std::string &/*name*/)
  {
  }

  virtual void markAsUsedImpl(::Persistency::IO::Transaction &/*t*/)
  {
  }

  virtual void markAsUnusedImpl(::Persistency::IO::Transaction &/*t*/)
  {
  }

  virtual void updateSeverityDeltaImpl(::Persistency::IO::Transaction &/*t*/, double /*delta*/)
  {
  }

  virtual void updateCertaintyDeltaImpl(::Persistency::IO::Transaction &/*t*/, double /*delta*/)
  {
  }

  virtual void addChildImpl(::Persistency::IO::Transaction &/*t*/, ::Persistency::MetaAlertPtrNN /*child*/)
  {
  }

  virtual void associateWithAlertImpl(::Persistency::IO::Transaction &/*t*/, ::Persistency::AlertPtrNN /*alert*/)
  {
  }
}; // class IOMetaAlertStub

} // namespace Persistency
} // namespace TestHelpers

#endif
