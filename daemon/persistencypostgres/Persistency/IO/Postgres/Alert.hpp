/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE

#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
// TODO: test

using namespace Persistency::IO::Postgres::detail;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief data base operations on Alert
 */
class Alert: public IO::Alert
{
public:
  /** \brief create alert persistency object for given alert.
   *  \param alert     alert to create object for.
   *  \param t         associated transaction.
   *  \param dbHandler shared connection to data base.
   */
  Alert(Persistency::AlertPtrNN  alert,
        Transaction             &t,
        DBHandlerPtrNN           dbHandler);

private:
  /** \brief save alert
   *  \param t  associated transaction.
   */
  virtual void saveImpl(Transaction &t);

  // TODO: use System::Enum<> for this.
  // TODO: keep names uppercase, camel-casing (ex: HostType)
  typedef enum host_type{SRC, DST} host_type;
  /** \brief save target or source hosts.
   *  \param es      EntrySaver object
   *  \param alertID ID of Alert
   *  \param type    type of saved Host (source or target).
   *  \param Hosts   Hosts to be saved.
   */
  void saveHosts(EntrySaver                        &es,
                 DataBaseID                        alertID,
                 host_type                         type,
                 Persistency::Alert::ReportedHosts &hosts);

  DBHandlerPtrNN dbHandler_;
}; // class Alert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
