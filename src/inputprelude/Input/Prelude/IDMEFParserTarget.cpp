/*
 * IDMEFParserTarget.cpp
 *
 */
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserCommons.hpp"
#include "Input/Prelude/IDMEFParserTarget.hpp"
#include "Persistency/Process.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;


IDMEFParserTarget::IDMEFParserTarget(idmef_target_t *ptr):
  ip_(parseIP(getNotNull(ptr))),
  process_(parseProcess(getNotNull(ptr))),
  service_(parseService(getNotNull(ptr)))
{
}

idmef_target_t * IDMEFParserTarget::getNotNull(idmef_target_t *ptr) const
{
  if (!ptr)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Idmef Target is empty.");
  return ptr;
}

IDMEFParserTarget::IP IDMEFParserTarget::parseIP(idmef_target_t *ptr) const
{
  idmef_node_t *idmef_node = idmef_target_get_node(ptr);
  const IDMEFParserTarget::IP *ip=IDMEFParserCommons::getIPfromIdmefNode(idmef_node).get();
  if (ip==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "No IP in source");
  return *ip;
}

Persistency::ProcessPtr IDMEFParserTarget::parseProcess(idmef_target_t * ptr) const
{
  idmef_user_t *idmef_user = idmef_target_get_user(ptr);
  idmef_process_t *idmef_process = idmef_target_get_process(ptr);

  return IDMEFParserCommons::getProcessfromIdmefProcess(idmef_process, idmef_user);
}

Persistency::ServicePtr IDMEFParserTarget::parseService(idmef_target_t * ptr) const
{
  idmef_service_t *idmef_service = idmef_target_get_service(ptr);
  return IDMEFParserCommons::getServicefromIdmefService(idmef_service);
}

const Persistency::Analyzer::IP & IDMEFParserTarget::getAddress() const
{
  return ip_;
}

const Persistency::ProcessPtr IDMEFParserTarget::getProcess() const
{
  return process_;
}

const Persistency::ServicePtr IDMEFParserTarget::getService() const
{
  return service_;
}

} // namespace Prelude
} // namespace Input


