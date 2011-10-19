/*
 * AnalyzersCreator.cpp
 *
 */
#include "DataFacades/AnalyzersCreator.hpp"
#include "DataFacades/detail/AnalyzersCreatorImpl.hpp"

namespace DataFacades
{

Persistency::AnalyzerPtrNN AnalyzersCreator::construct(Persistency::IO::ConnectionPtrNN              conn,
                                                       Persistency::IO::Transaction                 &t,
                                                       const Persistency::Analyzer::Name            &name,
                                                       const Persistency::Analyzer::Version         &version,
                                                       const Persistency::Analyzer::OperatingSystem &os,
                                                       const Persistency::Analyzer::IP              *ip)
{
  // compute hash
  const detail::LocalAnalyzersCache::Hash     hash=cache_.makeHash(name, version, os, ip);

  // try finding prper entry in cache
  detail::LocalAnalyzersCache::const_iterator it  =cache_.find(hash);
  if( it!=cache_.end() )
    return it->second;

  // if it's not prensent in local cache, create new object's instance
  detail::AnalyzersCreatorImpl impl(conn, t);                                       // make helper object
  Persistency::AnalyzerPtrNN   analyzer=impl.construct(hash, name, version, os, ip);// create analyzer
  cache_.insert( detail::LocalAnalyzersCache::value_type(hash, analyzer) );         // add entry to cache
  return analyzer;
}

} // namespace DataFacades
