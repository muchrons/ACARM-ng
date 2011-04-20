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

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN         conn,
                             const std::string                       &name,
                             Persistency::Facades::AnalyzersCreator  &creator,
                             const Persistency::IO::Heartbeats::Owner &heartbeatOwner):
  Core::Types::BackendFacade(conn, name),
  log_("input.backendfacade"),
  creator_(creator),
  heartbeatOwner_(heartbeatOwner)
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

void BackendFacade::heartbeat(const Persistency::IO::Heartbeats::Module &m, unsigned int deadline)
{
  LOGMSG_DEBUG_S(log_)<<"sending heartbeat from external module '"<<m.get()<<"' with deadline "<<deadline<<"[s]";
  beginTransaction();
  Persistency::IO::HeartbeatsAutoPtr hb=getConnection()->heartbeats( heartbeatOwner_, getTransaction() );
  assert( hb.get()!=NULL );
  hb->report(m, deadline);
  // transaction will be commited after everything's done
}

} // namespace Input
