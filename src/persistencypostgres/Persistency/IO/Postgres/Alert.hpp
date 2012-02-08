/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE

#include "Logger/Node.hpp"
#include "System/Enum.hpp"
#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"
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
   *  \param alert    alert to create object for.
   *  \param t        associated transaction.
   *  \param dbHandle shared connection to data base.
   */
  Alert(Persistency::AlertPtrNN  alert,
        Transaction             &t,
        DBHandlePtrNN            dbHandle);

private:
  /** \brief save alert
   *  \param t  associated transaction.
   */
  virtual void saveImpl(Transaction &t);

  struct HostEnumType
  {
    /** \brief type of host */
    typedef enum { SRC = 1, DST = 2 } Type;
  };

  typedef System::Enum<HostEnumType> HostType;
  /** \brief save target or source hosts.
   *  \param es      EntrySaver object
   *  \param alertID ID of Alert
   *  \param type    type of saved Host (source or target).
   *  \param Hosts   Hosts to be saved.
   */
  void saveHosts(EntrySaver                &es,
                 DataBaseID                 alertID,
                 HostType                   type,
                 Persistency::Alert::Hosts &hosts);
  Logger::Node  log_;
  DBHandlePtrNN dbHandle_;
}; // class Alert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
