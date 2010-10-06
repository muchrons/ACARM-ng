/*
 * AnalyzersCreator.cpp
 *
 */
#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Persistency/Facades/detail/AnalyzersCreatorImpl.hpp"

namespace Persistency
{
namespace Facades
{

AnalyzerPtrNN AnalyzersCreator::construct(IO::ConnectionPtrNN              conn,
                                          IO::Transaction                 &t,
                                          const Analyzer::Name            &name,
                                          const Analyzer::Version         &version,
                                          const Analyzer::OperatingSystem &os,
                                          const Analyzer::IP              *ip)
{
  // TODO               
  return AnalyzerPtrNN( new Analyzer(123u, name, version, os, ip) );
}

} // namespace Facades
} // namespace Persistency
