/*
 * LocalAnalyzersCache.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_DETAIL_LOCALANALYZERSCACHE_HPP_FILE
#define INCLUDE_DATAFACADES_DETAIL_LOCALANALYZERSCACHE_HPP_FILE

/* public header */

#include <map>
#include <string>

#include "Persistency/Analyzer.hpp"

namespace Facades
{
namespace detail
{

/** \brief cache for analyzers, kept in memory.
 */
struct LocalAnalyzersCache: public std::map<std::string, Persistency::AnalyzerPtrNN>
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
  Hash makeHash(const Persistency::Analyzer::Name            &name,
                const Persistency::Analyzer::Version         &version,
                const Persistency::Analyzer::OperatingSystem &os,
                const Persistency::Analyzer::IP              *ip) const;
}; // struct LocalAnalyzersCache

} // namespace detail
} // namespace Facades

#endif
