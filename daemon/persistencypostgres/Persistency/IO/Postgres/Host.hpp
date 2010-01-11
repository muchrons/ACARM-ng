/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_HOST_HPP_FILE

#include "Persistency/IO/Host.hpp"
#include "Persistency/IO/Postgres/DBSharedConnection.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief data base operations on Host
 */
class Host: public IO::Host
{
public:
  /** \brief create host persistency proxy object for given host.
   *  \param host host to work on.
   *  \param t    active transaction.
   *  \param sc   shared connection to data base.
   */
  Host(Persistency::HostPtrNN  host,
       const Transaction      &t,
       DBSharedConnection      sc);

private:
  virtual void setNameImpl(const Persistency::Host::Name &name);

  DBSharedConnection sc_;
}; // class Host

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
