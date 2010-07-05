/*
 * BackendFacade.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Input/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;

namespace Input
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                             const std::string                &name,
                             detail::AnalyzersMap             &analyzersMap,
                             CommonDataPtrNN                   commonData):
  Core::Types::BackendFacade(conn, name),
  analyzersMap_(analyzersMap),
  commonData_(commonData)
{
}

Persistency::AnalyzerPtrNN BackendFacade::getAnalyzer(const std::string                            &originalID,
                                                      const Persistency::Analyzer::Name            &name,
                                                      const Persistency::Analyzer::Version         &version,
                                                      const Persistency::Analyzer::OperatingSystem &os,
                                                      const Persistency::Analyzer::IP              *ip)
{
  // has a match?
  {
    AnalyzerPtrNN::SharedPtr analyzer=analyzersMap_.get(originalID);
    if( analyzer.get()!=NULL )
      return analyzer;
  }
  // new entry has to be created and added:
  beginTransaction();
  IDPersistencyProxyCommon   idppc( getConnection(), getTransaction() );
  IDPersistencyProxySpecific idpps( getConnection(), getTransaction(), getName() );
  // TODO: this unique ID should be created and added as a parameter to analyzer's c-tor.
  commonData_->getIDMap().get(idppc, idpps, originalID);  // TODO...
  //const Analyzer::ID         id=commonData_->getIDMap().get(idppc, idpps, originalID);  // assign some ID
  AnalyzerPtrNN              analyzer( new Analyzer(name, version, os, ip) );           // create new instance
  analyzersMap_.add(originalID, analyzer);                                              // add to cache

  // that's it!
  return analyzer;
}

} // namespace Input
