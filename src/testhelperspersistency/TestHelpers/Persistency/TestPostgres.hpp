/*
 * TestPostgres.cpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_TESTPOSTGRES_HPP
#define INCLUDE_TESTHELPERS_PERSISTENCY_TESTPOSTGRES_HPP

/* public header */

#include "TestHelpers/TestBase.hpp"

namespace TestHelpers
{
namespace Persistency
{

/** \brief base class for tests - creates config file for postgres.
 */
class TestPostgres: private TestBase
{
public:
  TestPostgres(void);
}; // struct TestPostgres

} // namespace Persistency
} // namespace TestHelpers

#endif
