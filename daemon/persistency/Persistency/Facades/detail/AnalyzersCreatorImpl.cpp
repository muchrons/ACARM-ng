/*
 * AnalyzersCreatorImpl.cpp
 *
 */
#include <cassert>

#include "Persistency/Facades/detail/AnalyzersCreatorImpl.hpp"
#include "Commons/Convert.hpp"

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

  // read saved value

  // TODO: ...
  return AnalyzerPtrNN( new Analyzer(1111u, name, version, os, ip) );
}

} // namespace detail
} // namespace Facades
} // namespace Persistency
