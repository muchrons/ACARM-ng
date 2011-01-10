/*
 * IDMEFParserSource.cpp
 *
 */
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserCommons.hpp"
#include "Input/Prelude/IDMEFParserSource.hpp"
#include "Persistency/Process.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;


IDMEFParserSource::IDMEFParserSource(idmef_source_t *ptr):
  ip_(parseIP(getNotNull(ptr))),
  process_(parseProcess(getNotNull(ptr))),
  service_(parseService(getNotNull(ptr)))
{
}

idmef_source_t * IDMEFParserSource::getNotNull(idmef_source_t *ptr) const
{
  if (!ptr)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Idmef Source is empty.");
  return ptr;
}

IDMEFParserSource::IP IDMEFParserSource::parseIP(idmef_source_t *ptr) const
{
  idmef_node_t *idmef_node = idmef_source_get_node(ptr);
  const IDMEFParserSource::IP *ip=IDMEFParserCommons::getIPfromIdmefNode(idmef_node).get();
  if (ip==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "No IP in source");
  return *ip;
}

Persistency::ProcessPtr IDMEFParserSource::parseProcess(idmef_source_t * ptr) const
{
  idmef_user_t *idmef_user = idmef_source_get_user(ptr);
  idmef_process_t *idmef_process = idmef_source_get_process(ptr);

  return IDMEFParserCommons::getProcessfromIdmefProcess(idmef_process, idmef_user);
}

Persistency::ServicePtr IDMEFParserSource::parseService(idmef_source_t * ptr) const
{
  idmef_service_t *idmef_service = idmef_source_get_service(ptr);
  return IDMEFParserCommons::getServicefromIdmefService(idmef_service);
}

const Persistency::Analyzer::IP & IDMEFParserSource::getAddress() const
{
  return ip_;
}

const Persistency::ProcessPtr IDMEFParserSource::getProcess() const
{
  return process_;
}

const Persistency::ServicePtr IDMEFParserSource::getService() const
{
  return service_;
}

} // namespace Prelude
} // namespace Input


