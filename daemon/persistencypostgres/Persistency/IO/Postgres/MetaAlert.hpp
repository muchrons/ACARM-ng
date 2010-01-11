/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE

#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/Postgres/DBSharedConnection.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief data base operations on meta-alert.
 */
class MetaAlert: public IO::MetaAlert
{
public:
  /** \brief create meta-alert persistency proxy.
   *  \param ma meta alert to work on.
   *  \param t  active transaction.
   *  \param sc shared connection ot be used for communication.
   */
  MetaAlert(Persistency::MetaAlertPtrNN  ma,
            const Transaction           &t,
            DBSharedConnection           sc);

private:
  virtual void saveImpl(void);
  virtual void markAsUsedImpl(void);
  virtual void markAsUnusedImpl(void);
  virtual void updateSeverityDeltaImpl(double delta);
  virtual void updateCertanityDeltaImpl(double delta);
  virtual void addChildImpl(Persistency::MetaAlertPtrNN child);
  virtual void associateWithAlertImpl(Persistency::AlertPtrNN alert);

  DBSharedConnection sc_;
}; // class MetaAlert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
