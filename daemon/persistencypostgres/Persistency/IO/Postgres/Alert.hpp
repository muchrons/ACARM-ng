/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_ALERT_HPP_FILE

#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/Postgres/DBSharedConnection.hpp"

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
   *  \param alert alert to create object for.
   *  \param t     associated transaction.
   *  \param sc    shared connection to data base.
   */
  Alert(Persistency::AlertPtrNN  alert,
        const Transaction       &t,
        DBSharedConnection       sc);

private:
  virtual void saveImpl(void);

  DBSharedConnection sc_;
}; // class Alert

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
