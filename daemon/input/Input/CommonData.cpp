/*
 * CommonData.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Input/CommonData.hpp"

namespace Input
{

namespace
{
Persistency::Analyzer::ID readNextFreeID(Persistency::IO::DynamicConfig &dynConf)
{
  Logger::Node log("input.commondata.readnextfreeid");
  LOGMSG_DEBUG(log, "reading next free ID from data base");
  const Persistency::Analyzer::ID out=
} // readNextFreeID()
} // namespace

CommonData::CommonData(Persistency::IO::DynamicConfig &dynConf):
  idMap_( readNextFreeID(dynConf) )
{
}

} // namespace Input
