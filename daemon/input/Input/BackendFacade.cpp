/*
 * BackendFacade.cpp
 *
 */
#include "Input/BackendFacade.hpp"

using namespace Persistency;

namespace Input
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN        conn,
                             const std::string                      &name,
                             Persistency::Facades::AnalyzersCreator &creator):
  Core::Types::BackendFacade(conn, name),
  creator_(creator)
{
}

Persistency::AnalyzerPtrNN BackendFacade::getAnalyzer(const Persistency::Analyzer::Name            &name,
                                                      const Persistency::Analyzer::Version         &version,
                                                      const Persistency::Analyzer::OperatingSystem &os,
                                                      const Persistency::Analyzer::IP              *ip)
{
  beginTransaction();
  return creator_.construct( getConnection(), getTransaction(), name, version, os, ip );
}

} // namespace Input
