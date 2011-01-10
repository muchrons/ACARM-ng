/*
 * TestConnection.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCONNECTION_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TESTCONNECTION_T_HPP_FILE

#include "Persistency/IO/Postgres/DBHandle.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

struct TestConnection
{
  TestConnection(void):
    connParams_( makeParams() ),
    idCache_(new IDCache),
    dbHandle_(connParams_, idCache_)
  {
  }

  static DBConnection::Parameters makeParams(void)
  {
    return DBConnection::Parameters("localhost",
                                    "5432",
                                    "acarm_ng_test",
                                    "acarm-ng-daemon",
                                    "test.daemon");
  }

  DBConnection::Parameters connParams_;
  IDCachePtrNN             idCache_;
  DBHandle                 dbHandle_;
}; // struct TestConnection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
