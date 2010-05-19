/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE

#include "Logger/Node.hpp"
#include "System/Enum.hpp"
#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/Postgres/ExceptionPQXX.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

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

  struct HostEnumType
  {
    typedef enum{ SRC = 1, DST = 2 } Type;
  };

  typedef System::Enum<HostEnumType> HostType;
  /** \brief save target or source hosts.
   *  \param es      EntrySaver object
   *  \param alertID ID of Alert
   *  \param type    type of saved Host (source or target).
   *  \param Hosts   Hosts to be saved.
   */
  void saveHosts(EntrySaver                        &es,
                 DataBaseID                        alertID,
                 HostType                          type,
                 Persistency::Alert::ReportedHosts &hosts);
  Logger::Node   log_;
  DBHandlerPtrNN dbHandler_;
}; // class Alert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
