/*
 * Connection.cpp
 *
 */
#include "Persistency/IO/Postgres/Connection.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

Connection::Connection(const std::string &,//server,
                       const std::string &,//dbname,
                       const std::string &,//user,
                       const std::string &)//pass)
{
  // TODO
}



TransactionAPIAutoPtr Connection::createNewTransactionImpl(
                                                Base::Threads::Mutex &,//mutex,
                                                const std::string    &)//name)
{
  // TODO
  return Persistency::IO::TransactionAPIAutoPtr();
}

AlertAutoPtr Connection::alertImpl(AlertPtr           /*alert*/,
                                   const Transaction &/*t*/)
{
  // TODO
  return Persistency::IO::AlertAutoPtr();
}

HostAutoPtr Connection::hostImpl(HostPtr            /*host*/,
                                 const Transaction &/*t*/)
{
  // TODO
  return Persistency::IO::HostAutoPtr();
}

MetaAlertAutoPtr Connection::metaAlertImpl(MetaAlertPtr       /*ma*/,
                                           const Transaction &/*t*/)
{
  // TODO
  return MetaAlertAutoPtr();
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
