/*
 * DynamicConfig.mt.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"

using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

int main(int argc, char **argv)
{
  const char *owner=NULL;
  const char *key  =NULL;
  const char *value=NULL;

  switch(argc)
  {
    case 1+2:
      owner=NULL;
      key  =argv[1];
      value=argv[2];
      break;

    case 1+3:
      owner=argv[1];
      key  =argv[2];
      value=argv[3];
      break;

    default:
      std::cerr<<argv[0]<<" <key> <value>"<<std::endl;
      std::cerr<<argv[0]<<" <owner> <key> <value>"<<std::endl;
      return 1;
  } // switch(argc)

  IO::ConnectionPtrNN      conn( IO::create() );
  Transaction              t( conn->createNewTransaction("dynamic_config_manlual_test" ) );
  IO::DynamicConfigAutoPtr dc( conn->dynamicConfig(owner, t) );
  assert( dc.get()!=NULL );
  dc->write(key, value);
  t.commit();

  return 0;
}
