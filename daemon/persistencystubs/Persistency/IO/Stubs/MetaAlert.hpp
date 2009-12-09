/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_METAALERT_HPP_FILE

#include "Persistency/IO/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief operations on Persistency::MetaAlert API
 */
class MetaAlert: public IO::MetaAlert
{
public:
  /** \brief meta alert persistency IO.
   *  \param ma      meta alert to work on.
   *  \param t       active transaction.
   *  \param handler persistency handler.
   */
  MetaAlert(Persistency::MetaAlertPtr  ma,
            const Transaction         &t,
            int                        handler);

private:
  virtual void saveImpl();
  virtual void markAsUsedImpl();
  virtual void markAsUnusedImpl();
  virtual void updateSeverityDeltaImpl(double delta);
  virtual void updateCertanityDeltaImpl(double delta);
  virtual void addChildImpl(Persistency::MetaAlertPtr child);
  virtual void associateWithAlertImpl(Persistency::AlertPtr alert);
}; // class MetaAlert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
