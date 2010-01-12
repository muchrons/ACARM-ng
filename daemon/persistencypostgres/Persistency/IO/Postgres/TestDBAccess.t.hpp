/*
 * TestDBAccress.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TESTDBACCESS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TESTDBACCESS_T_HPP_FILE

#include "Persistency/IO/Postgres/DBConnection.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

struct TestDBAccess
{
  TestDBAccess(void);

  void removeAllData(void);

  void fillWithContent1(void);

private:
  DBConnection conn_;
}; // struct TestDBAccess

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
