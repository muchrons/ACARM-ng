/*
 * BackendFacade.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Input/BackendFacade.hpp"

using namespace Persistency;

namespace Input
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN       conn,
                             const Core::Types::Proc::TypeName     &type,
                             const Core::Types::Proc::InstanceName &name,
                             DataFacades::AnalyzersCreator         &creator):
  Core::Types::BackendFacade(conn, Core::Types::Proc::CategoryName("input"), type, name),
  log_("input.backendfacade"),
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
