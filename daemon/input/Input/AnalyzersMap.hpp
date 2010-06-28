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

class AnalyzersMap: private boost::noncopyable
{
public:
  explicit AnalyzersMap(const std::string &inputName);

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
