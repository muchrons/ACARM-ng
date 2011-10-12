/*
 * AnalyzersCreatorImpl.cpp
 *
 */
#include <cassert>

#include "DataFacades/detail/AnalyzersCreatorImpl.hpp"
#include "Commons/Convert.hpp"
#include "DataFacades/IDAssigner.hpp"

namespace Persistency
{
namespace Facades
{
namespace detail
{

AnalyzersCreatorImpl::AnalyzersCreatorImpl(IO::ConnectionPtrNN conn, IO::Transaction &t):
  conn_(conn),
  t_(t),
  dc_( conn_->dynamicConfig("Persistency::Facades::AnalyzersCreator", t_) )
{
  assert( dc_.get()!=NULL );
}

AnalyzerPtrNN AnalyzersCreatorImpl::construct(const LocalAnalyzersCache::Hash &hash,
                                              const Analyzer::Name            &name,
                                              const Analyzer::Version         &version,
                                              const Analyzer::OperatingSystem &os,
                                              const Analyzer::IP              *ip)
{
  assert( dc_.get()!=NULL );
  assert( hash==LocalAnalyzersCache().makeHash(name, version, os, ip) );

  // read saved ID for a given analyzer
  const IO::DynamicConfig::ValueNULL r=dc_->read(hash);
  if( r.get()!=NULL )
  {
    const Analyzer::ID id( Commons::Convert::to<Analyzer::ID::Numeric>( r.get()->get() ) );
    return AnalyzerPtrNN( new Analyzer(id, name, version, os, ip) );
  }

  // if ID is not assigned to this object yet, assign one as save it
  const Analyzer::ID id( Facades::IDAssigner::get()->assignAnalyzerID(conn_, t_) ); // assign
  dc_->write( hash, Commons::Convert::to<std::string>( id.get() ) );                // save mapping
  return AnalyzerPtrNN( new Analyzer(id, name, version, os, ip) );                  // return object
}

} // namespace detail
} // namespace Facades
} // namespace Persistency
