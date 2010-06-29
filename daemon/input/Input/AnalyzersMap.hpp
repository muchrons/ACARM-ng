/*
 * AnalyzersMap.hpp
 *
 */
#ifndef INCLUDE_INPUT_ANALYZERSMAP_HPP_FILE
#define INCLUDE_INPUT_ANALYZERSMAP_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"

// TODO

namespace Input
{

/** \brief main mapping object - maps 
 */
class AnalyzersMap: private boost::noncopyable
{
public:
  /** \brief gets mapping from given originalID to analyzer. if ID's not mapped, new entry's added.
   *  \param originalID ID returned by input.
   *  \param name       analyzer's name.
   *  \param version    analyzer's version.
   *  \param os         analyzer's OS.
   *  \param ip         analyzer's IP.
   *  \return analyzer's object instance.
   */
  Persistency::AnalyzerPtrNN get(const std::string                            &originalID,
                                 const Persistency::Analyzer::Name            &name,
                                 const Persistency::Analyzer::Version         &version,
                                 const Persistency::Analyzer::OperatingSystem &os,
                                 const Persistency::Analyzer::IP              *ip);

private:
  const std::string inputName_;
}; // class AnalyzersMap

} // namespace Input

#endif
