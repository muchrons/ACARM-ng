/*
 * AnalyzersMap.cpp
 *
 */
#include "Input/AnalyzersMap.hpp"

namespace Input
{

AnalyzersMap::AnalyzersMap(const std::string &inputName):
  inputName_(inputName)
{
}

Persistency::AnalyzerPtrNN AnalyzersMap::get(const std::string                            &/*originalID*/,
                                             const Persistency::Analyzer::Name            &name,
                                             const Persistency::Analyzer::Version         &version,
                                             const Persistency::Analyzer::OperatingSystem &os,
                                             const Persistency::Analyzer::IP              *ip)
{
  // TODO
  return Persistency::AnalyzerPtrNN( new Persistency::Analyzer(name, version, os, ip) );
}

} // namespace Input
