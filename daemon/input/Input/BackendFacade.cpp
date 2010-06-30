/*
 * BackendFacade.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Input/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;

// TODO

namespace Input
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                             const std::string                &name,
                             detail::AnalyzersMap             &analyzersMap):
  Core::Types::BackendFacade(conn, "Input::"+name),
  analyzersMap_(analyzersMap)
{
  // TODO: external map of IDs must be priovided; each input should take it as a paramter
  //       during creation (create()) so that it could be external element.
}

Persistency::AnalyzerPtrNN BackendFacade::getAnalyzer(const std::string                            &/*originalID*/,
                                                      const Persistency::Analyzer::Name            &name,
                                                      const Persistency::Analyzer::Version         &version,
                                                      const Persistency::Analyzer::OperatingSystem &os,
                                                      const Persistency::Analyzer::IP              *ip)
{
  //const string mixedID=getName() + "::" + iriginalID;
  // TODO
  return Persistency::AnalyzerPtrNN( new Persistency::Analyzer(name, version, os, ip) );
}

void BackendFacade::saveMapping(const std::string &inputID, Persistency::Analyzer::ID id)
{
  DynamicConfigAutoPtr dc=getConnection()->dynamicConfig( getName(), getTransaction() );
  assert( dc.get()!=NULL );
  dc->write( inputID, boost::lexical_cast<string>(id) );
}

Persistency::Analyzer::ID BackendFacade::readNextFreeID(void)
{
  DynamicConfigAutoPtr     dc =getConnection()->dynamicConfig( "Input", getTransaction() );
  assert( dc.get()!=NULL );
  DynamicConfig::ValueNULL tmp=dc->read("next free ID");
  // no value set? assume counter has ust started.
  if( tmp.get()==NULL )
    return 0;
  // parse input value and return to caller
  const Persistency::Analyzer::ID out=boost::lexical_cast<Persistency::Analyzer::ID>( tmp.get()->get() );
  return out;
}

void BackendFacade::saveNextFreeID(Persistency::Analyzer::ID nextFreeID)
{
  DynamicConfigAutoPtr dc=getConnection()->dynamicConfig( "Input", getTransaction() );
  assert( dc.get()!=NULL );
  dc->write( "next free ID", boost::lexical_cast<string>(nextFreeID) );
}

} // namespace Input
