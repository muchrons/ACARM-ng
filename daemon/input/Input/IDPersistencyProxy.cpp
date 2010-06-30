/*
 * IDPersistencyProxy.cpp
 *
 */
#include <boost/lexical_cast.hpp>

#include "Persistency/IO/DynamicConfig.hpp"
#include "Input/IDPersistencyProxy.hpp"

using namespace std;
using namespace Persistency::IO;

namespace Input
{

namespace
{
const char *ownerName="Input";
} // unnamed namespace

IDPersistencyProxy::IDPersistencyProxy(Persistency::IO::ConnectionPtrNN  conn,
                                       Persistency::IO::Transaction     &t):
  log_("input.idpersistencyproxy"),
  conn_(conn),
  t_(t)
{
}

void IDPersistencyProxy::saveMapping(const std::string &inputID, Persistency::Analyzer::ID id)
{
  DynamicConfigAutoPtr dc=conn_->dynamicConfig(ownerName , t_);
  assert( dc.get()!=NULL );
  dc->write( inputID, boost::lexical_cast<string>(id) );
}

Persistency::Analyzer::ID IDPersistencyProxy::readNextFreeID(void)
{
  DynamicConfigAutoPtr     dc =conn_->dynamicConfig(ownerName, t_);
  assert( dc.get()!=NULL );
  DynamicConfig::ValueNULL tmp=dc->read("next free ID");
  // no value set? assume counter has ust started.
  if( tmp.get()==NULL )
    return 0;
  // parse input value and return to caller
  const Persistency::Analyzer::ID out=boost::lexical_cast<Persistency::Analyzer::ID>( tmp.get()->get() );
  return out;
}

void IDPersistencyProxy::saveNextFreeID(Persistency::Analyzer::ID nextFreeID)
{
  DynamicConfigAutoPtr dc=conn_->dynamicConfig(ownerName, t_);
  assert( dc.get()!=NULL );
  dc->write( "next free ID", boost::lexical_cast<string>(nextFreeID) );
}

} // namespace Input
