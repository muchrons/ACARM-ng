/*
 * Analyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_ANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_ANALYZER_HPP_FILE

/* public header */

#include "Persistency/Analyzer.hpp"
#include "Persistency/Stubs/Host.hpp"

namespace Persistency
{
namespace Stubs
{
/** \brief class representing analyzer's info.
 */
class Analyzer: public Persistency::Analyzer
{
public:
  /** \brief creates analyzer.
   *  \param name name of an analyzer.
   *  \param host host analyzer runs on or NULL if not known.
   */
  Analyzer(const Name &name,
           HostPtr     host):
    Persistency::Analyzer(name, host)
  {
  }
  /** \brief creates default analyzer.
   */
  Analyzer(void):
    Persistency::Analyzer("analyzer 1",
                          HostPtr(new Host) )
  {
  }
}; // class Analyzer

} // namespace Stubs
} // namespace Persistency

#endif
