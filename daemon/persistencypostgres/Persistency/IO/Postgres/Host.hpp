/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_HOST_HPP_FILE

#include "Persistency/IO/Host.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

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
   *  \param host      host to work on.
   *  \param t         active transaction.
   *  \param dbHandler shared connection to data base.
   */
  Host(Persistency::HostPtrNN  host,
       Transaction            &t,
       DBHandlerPtrNN          dbHandler);

private:
  virtual void setNameImpl(Transaction &t, const Persistency::Host::Name &name);

  DBHandlerPtrNN dbHandler_;
}; // class Host

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
