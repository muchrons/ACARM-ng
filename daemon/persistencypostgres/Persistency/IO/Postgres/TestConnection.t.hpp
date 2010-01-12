/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCONNECTION_T_HPP_FILE

#include "Persistency/IO/Postgres/DBHandler.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

struct TestConnection
{
  TestConnection(void):
    connParams_("localhost", "acarm_ng_test", "acarm-ng-daemon", "test.daemon"),
    idCache_(new IDCache),
    dbHandler_(connParams_, idCache_)
  {
  }

  DBConnection::Parameters connParams_;
  IDCachePtrNN             idCache_;
  DBHandler                dbHandler_;
}; // struct TestConnection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
