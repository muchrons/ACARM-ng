/*
 * TestDBAccress.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_TESTDBACCESS_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_TESTDBACCESS_HPP_FILE

/* public header */

#include <pqxx/pqxx>

namespace TestHelpers
{
namespace Persistency
{

struct TestDBAccess
{
  TestDBAccess(void);

  void removeAllData(void);

  void markAllUnused(void);

  void fillWithContent1(void);

private:
  pqxx::connection conn_;
}; // struct TestDBAccess

} // namespace Persistency
} // namespace TestHelpers

#endif
