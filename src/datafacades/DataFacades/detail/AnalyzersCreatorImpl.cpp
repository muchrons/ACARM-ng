/*
 * AnalyzersCreatorImpl.cpp
 *
 */
#include <cassert>

#include "DataFacades/detail/AnalyzersCreatorImpl.hpp"
#include "Commons/Convert.hpp"
#include "DataFacades/IDAssigner.hpp"

using namespace Persistency;

namespace DataFacades
{
namespace detail
{

AnalyzersCreatorImpl::AnalyzersCreatorImpl(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t):
  conn_(conn),
  t_(t),
  dc_( conn_->dynamicConfig("DataFacades::AnalyzersCreator", t_) )
{
  assert( dc_.get()!=NULL );
}

Persistency::AnalyzerPtrNN AnalyzersCreatorImpl::construct(const LocalAnalyzersCache::Hash              &hash,
                                                           const Persistency::Analyzer::Name            &name,
                                                           const Persistency::Analyzer::Version         &version,
                                                           const Persistency::Analyzer::OperatingSystem &os,
                                                           const Persistency::Analyzer::IP              *ip)
{
  assert( dc_.get()!=NULL );
  assert( hash==LocalAnalyzersCache().makeHash(name, version, os, ip) );

  // read saved ID for a given analyzer
  const Persistency::IO::DynamicConfig::ValueNULL r=dc_->read(hash);
  if( r.get()!=NULL )
  {
    const Analyzer::ID id( Commons::Convert::to<Analyzer::ID::Numeric>( r.get()->get() ) );
    return AnalyzerPtrNN( new Analyzer(id, name, version, os, ip) );
  }

  // if ID is not assigned to this object yet, assign one as save it
  const Analyzer::ID id( DataFacades::IDAssigner::get()->assignAnalyzerID(conn_, t_) ); // assign
  dc_->write( hash, Commons::Convert::to<std::string>( id.get() ) );                    // save mapping
  return AnalyzerPtrNN( new Analyzer(id, name, version, os, ip) );                      // return object
}

} // namespace detail
} // namespace DataFacades
