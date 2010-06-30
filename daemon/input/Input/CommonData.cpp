/*
 * CommonData.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Input/CommonData.hpp"
#include "Input/IDPersistencyProxyCommon.hpp"

namespace Input
{

namespace
{
Persistency::Analyzer::ID readNextFreeID(Persistency::IO::ConnectionPtrNN  conn,
                                         Persistency::IO::Transaction     &t)

{
  Logger::Node                    log("input.commondata.readnextfreeid");
  LOGMSG_DEBUG(log, "reading next free ID from data base");
  IDPersistencyProxyCommon        ppc(conn, t);
  const Persistency::Analyzer::ID out=ppc.readNextFreeID();
  LOGMSG_DEBUG_S(log)<<"read 'next free ID' paramter's value: "<<out;
  return out;
} // readNextFreeID()
} // namespace

CommonData::CommonData(Persistency::IO::ConnectionPtrNN  conn,
                       Persistency::IO::Transaction     &t):
  idMap_( readNextFreeID(conn, t) )
{
}

} // namespace Input
