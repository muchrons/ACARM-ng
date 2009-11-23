/*
 * Analyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_ANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_ANALYZER_HPP_FILE

#include "Persistency/Analyzer.hpp"

namespace Persistency
{
namespace Postgres
{
/** \brief class representing analyzer's info.
 */
class Analyzer: public Persistency::Analyzer
{
public:
  /** \brief helper typedef to make declaration shorter.
   */
  typedef Persistency::Analyzer AnalyzerAPI;

  /** \brief creates analyzer.
   *  \param name name of an analyzer.
   *  \param host host analyzer runs on or NULL if not known.
   */
  Analyzer(const AnalyzerAPI::Name &name,
           HostPtr                  host);
}; // class Analyzer

} // namespace Postgres
} // namespace Persistency

#endif
