/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE

#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"

// TODO: test

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
   *  \param ma        meta alert to work on.
   *  \param t         active transaction.
   *  \param dbHandler shared connection ot be used for communication.
   */
  MetaAlert(Persistency::MetaAlertPtrNN  ma,
            Transaction                 &t,
            DBHandlerPtrNN               dbHandler);

private:
  virtual void saveImpl(Transaction &t);
  virtual void markAsUsedImpl(Transaction &t);
  virtual void markAsUnusedImpl(Transaction &t);
  virtual void updateSeverityDeltaImpl(Transaction &t, double delta);
  virtual void updateCertaintyDeltaImpl(Transaction &t, double delta);
  virtual void addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child);
  virtual void associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert);

  DBHandlerPtrNN dbHandler_;
}; // class MetaAlert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
