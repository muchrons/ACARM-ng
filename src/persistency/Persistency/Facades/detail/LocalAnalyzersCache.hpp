/*
 * LocalAnalyzersCache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_DETAIL_LOCALANALYZERSCACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_DETAIL_LOCALANALYZERSCACHE_HPP_FILE

/* public header */

#include <map>
#include <string>

#include "Persistency/Analyzer.hpp"

namespace Persistency
{
namespace Facades
{
namespace detail
{

/** \brief cache for analyzers, kept in memory.
 */
struct LocalAnalyzersCache: public std::map<std::string, AnalyzerPtrNN>
{
  /** \brief hash value. */
  typedef std::string Hash;

  /** \brief hash-computing function.
   *  \param name    analyzer's name.
   *  \param version analyzer's version.
   *  \param os      analyzer's operating system name.
   *  \param ip      analyzer's IP.
   *  \return hash of a given paramters.
   */
  Hash makeHash(const Analyzer::Name            &name,
                const Analyzer::Version         &version,
                const Analyzer::OperatingSystem &os,
                const Analyzer::IP              *ip) const;
}; // struct LocalAnalyzersCache

} // namespace detail
} // namespace Facades
} // namespace Persistency

#endif
