/*
 * AnalyzersCreatorImpl.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_DETAIL_ANALYZERSCREATORIMPL_HPP_FILE
#define INCLUDE_DATAFACADES_DETAIL_ANALYZERSCREATORIMPL_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"
#include "Persistency/IO/Connection.hpp"
#include "DataFacades/detail/LocalAnalyzersCache.hpp"

namespace Facades
{
namespace detail
{

/** \brief implementation for AnalyzersCreator that simplifies API a bit.
 */
class AnalyzersCreatorImpl: private boost::noncopyable
{
public:
  /** \brief create helper object.
   *  \param conn connection use for I/O operations, if needed.
   *  \param t    transaction to oeprate on.
   */
  AnalyzersCreatorImpl(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

  /** \brief creates analyzer object, based on provided data, or creates new, if it did not exist yet.
   *  \param hash    hash of a given values.
   *  \param name    analyzer's name.
   *  \param version analyzer's version.
   *  \param os      analyzer's OS.
   *  \param ip      analyzer's IP.
   *  \return analyzer's object instance.
   *
   *  'hash' paramter is passed only as an optimization, since it can be computed from a given
   *  parameters as well, but since our caller have already done this, we can use it here as well.
   */
  Persistency::AnalyzerPtrNN construct(const LocalAnalyzersCache::Hash              &hash,
                                       const Persistency::Analyzer::Name            &name,
                                       const Persistency::Analyzer::Version         &version,
                                       const Persistency::Analyzer::OperatingSystem &os,
                                       const Persistency::Analyzer::IP              *ip);

private:
  Persistency::IO::ConnectionPtrNN       conn_;
  Persistency::IO::Transaction          &t_;
  Persistency::IO::DynamicConfigAutoPtr  dc_;
}; // class AnalyzersCreatorImpl

} // namespace detail
} // namespace Facades

#endif
