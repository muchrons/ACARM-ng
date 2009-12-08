/*
 * Connection.cpp
 *
 */
#include "Persistency/IO/Stubs/Connection.hpp"
#include "Persistency/IO/Stubs/TransactionAPI.hpp"
#include "Persistency/IO/Stubs/Alert.hpp"
#include "Persistency/IO/Stubs/Graph.hpp"
#include "Persistency/IO/Stubs/Host.hpp"
#include "Persistency/IO/Stubs/MetaAlert.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

Connection::Connection(const std::string &,//server,
                       const std::string &,//dbname,
                       const std::string &,//user,
                       const std::string &)//pass)
{
  // TODO
}



TransactionAPIAutoPtr Connection::createNewTransactionImpl(
                                                Base::Threads::Mutex &mutex,
                                                const std::string    &name)
{
  return TransactionAPIAutoPtr( new Stubs::TransactionAPI(mutex, name) );
}

AlertAutoPtr Connection::alertImpl(AlertPtr           alert,
                                   const Transaction &t)
{
  return AlertAutoPtr( new Stubs::Alert(alert, t) );
}

GraphAutoPtr Connection::graphImpl(const Transaction &t)
{
  return GraphAutoPtr( new Stubs::Graph(t) );
}

HostAutoPtr Connection::hostImpl(HostPtr            host,
                                 const Transaction &t)
{
  return HostAutoPtr( new Stubs::Host(host, t) );
}

MetaAlertAutoPtr Connection::metaAlertImpl(MetaAlertPtr       ma,
                                           const Transaction &t)
{
  return MetaAlertAutoPtr( new Stubs::MetaAlert(ma, t) );
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
