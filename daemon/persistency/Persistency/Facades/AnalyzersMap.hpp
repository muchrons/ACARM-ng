/*
 * AnalyzersMap.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_ANALYZERSMAP_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_ANALYZERSMAP_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"

namespace Persistency
{
namespace Facades
{

/** \brief 
 */
class AnalyzersMap: private boost::noncopyable
{
public:
  /** \brief gets analyzer, based on provided data, or creates new, if it did not exist yet.
   *  \param name       analyzer's name.
   *  \param version    analyzer's version.
   *  \param os         analyzer's OS.
   *  \param ip         analyzer's IP.
   *  \return analyzer's object instance.
   */
  AnalyzerPtrNN getAnalyzer(const Analyzer::Name            &name,
                            const Analyzer::Version         &version,
                            const Analyzer::OperatingSystem &os,
                            const Analyzer::IP              *ip);
}; // class Analyzer

} // namespace Facades
} // namespace Persistency

#endif
