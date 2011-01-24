/*
 * AnalyzersCreator.cpp
 *
 */
#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Persistency/Facades/detail/AnalyzersCreatorImpl.hpp"

namespace Persistency
{
namespace Facades
{

AnalyzerPtrNN AnalyzersCreator::construct(IO::ConnectionPtrNN              conn,
                                          IO::Transaction                 &t,
                                          const Analyzer::Name            &name,
                                          const Analyzer::Version         &version,
                                          const Analyzer::OperatingSystem &os,
                                          const Analyzer::IP              *ip)
{
  // compute hash
  const detail::LocalAnalyzersCache::Hash     hash=cache_.makeHash(name, version, os, ip);

  // try finding prper entry in cache
  detail::LocalAnalyzersCache::const_iterator it  =cache_.find(hash);
  if( it!=cache_.end() )
    return it->second;

  // if it's not prensent in local cache, create new object's instance
  detail::AnalyzersCreatorImpl impl(conn, t);                                       // make helper object
  AnalyzerPtrNN                analyzer=impl.construct(hash, name, version, os, ip);// create analyzer
  cache_.insert( detail::LocalAnalyzersCache::value_type(hash, analyzer) );         // add entry to cache
  return analyzer;
}

} // namespace Facades
} // namespace Persistency
