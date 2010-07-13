/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_METAALERT_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"

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
   *  \param ma       meta alert to work on.
   *  \param t        active transaction.
   *  \param dbHandle shared connection ot be used for communication.
   */
  MetaAlert(Persistency::MetaAlertPtrNN  ma,
            Transaction                 &t,
            DBHandlePtrNN                dbHandle);

private:
  virtual void saveImpl(Transaction &t);
  virtual void markAsTriggeredImpl(Transaction &t, const std::string &name);
  virtual void markAsUsedImpl(Transaction &t);
  virtual void markAsUnusedImpl(Transaction &t);
  virtual void updateSeverityDeltaImpl(Transaction &t, double delta);
  virtual void updateCertaintyDeltaImpl(Transaction &t, double delta);
  virtual void addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child);
  virtual void associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert);
  DataBaseID getID();

  Logger::Node  log_;
  DBHandlePtrNN dbHandle_;
}; // class MetaAlert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
