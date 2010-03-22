/*
 * PqxxLeakTest.mt.cpp
 *
 */
#include "Persistency/IO/Postgres/DBConnection.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"

using namespace Persistency::IO::Postgres;

int main(void)
{
  DBConnection c( TestConnection::makeParams() );
  return 0;
}
