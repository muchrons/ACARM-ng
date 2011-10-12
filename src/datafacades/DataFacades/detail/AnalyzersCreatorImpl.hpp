/*
 * AnalyzersCreatorImpl.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_DETAIL_ANALYZERSCREATORIMPL_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_DETAIL_ANALYZERSCREATORIMPL_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"
#include "Persistency/IO/Connection.hpp"
#include "DataFacades/detail/LocalAnalyzersCache.hpp"

namespace Persistency
{
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
  AnalyzersCreatorImpl(IO::ConnectionPtrNN conn, IO::Transaction &t);

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
  AnalyzerPtrNN construct(const LocalAnalyzersCache::Hash &hash,
                          const Analyzer::Name            &name,
                          const Analyzer::Version         &version,
                          const Analyzer::OperatingSystem &os,
                          const Analyzer::IP              *ip);

private:
  IO::ConnectionPtrNN       conn_;
  IO::Transaction          &t_;
  IO::DynamicConfigAutoPtr  dc_;
}; // class AnalyzersCreatorImpl

} // namespace detail
} // namespace Facades
} // namespace Persistency

#endif
