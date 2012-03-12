/*
 * IDMEFParserCommons.cpp
 *
 */
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserCommons.hpp"
#include "Persistency/Process.hpp"
#include "Logger/Logger.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;


Base::NullValue<IDMEFParserCommons::IP> IDMEFParserCommons::getIPfromIdmefNode(idmef_node_t * idmef_node)
{
  if (idmef_node==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Idmef Node is empty.");

  idmef_address_t *idmef_node_addr = idmef_node_get_next_address(idmef_node, NULL);
  if (idmef_node_addr==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Idmef Node->address is empty.");

  const prelude_string_t *idmef_node_address = idmef_address_get_address(idmef_node_addr);
  if (idmef_node_address==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "String parse error, no address extracted.");

  const char * tmp=prelude_string_get_string(idmef_node_address);
  switch (idmef_address_get_category(idmef_node_addr))
  {
    case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR:
    case IDMEF_ADDRESS_CATEGORY_IPV4_NET:
      return Base::NullValue<IDMEFParserCommons::IP>(address_v4::from_string(tmp));

    case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
    case IDMEF_ADDRESS_CATEGORY_IPV6_NET:
      return Base::NullValue<IDMEFParserCommons::IP>(address_v6::from_string(tmp));

    default:
      throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Wrong address type.");
  }
  return Base::NullValue<IDMEFParserCommons::IP>(NULL);
}

Persistency::ServicePtr IDMEFParserCommons::getServicefromIdmefService(idmef_service_t * idmef_service)
{
  Persistency::ServicePtr service;
  if (idmef_service)
  {
    prelude_string_t *idmef_service_name = idmef_service_get_name(idmef_service);
    const Persistency::Service::Name name=(idmef_service_name==NULL)?"unknown":(prelude_string_get_string(idmef_service_name));

    Persistency::Service::Protocol protocol;
    const prelude_string_t *idmef_protocol_str = idmef_service_get_protocol(idmef_service);
    if (idmef_protocol_str)
      protocol=prelude_string_get_string(idmef_protocol_str);

    const uint16_t *idmef_port = idmef_service_get_port(idmef_service);

    uint16_t port=0;
    if (idmef_port!=NULL)
      port=*idmef_port;

    if (port==0)
      {
        //logger node is created here because this is a static method and no constructor is ever called
        Logger::Node log_( Logger::NodeName( "input.prelude.idmefparsercommons") );
        LOGMSG_DEBUG_S(log_)<<"No port specified in a service";
        return service;
      }

    service.reset(new Persistency::Service(name,port,protocol,Persistency::ReferenceURLPtr()));
  }
  return service;
}

Persistency::ProcessPtr IDMEFParserCommons::getProcessfromIdmefProcess(idmef_process_t * idmef_process, idmef_user_t * idmef_user)
{
  Persistency::Process::Username username;
  if (idmef_user!=NULL)
  {
    // get the first available user from the list if there's any
    idmef_user_id_t *idmef_user_id = idmef_user_get_next_user_id(idmef_user, NULL);
    if (idmef_user_id!=NULL)
    {
      prelude_string_t *user_id_str = idmef_user_id_get_name(idmef_user_id);
      if (user_id_str!=NULL)
        username=prelude_string_get_string(user_id_str);
    }
  }

  Persistency::ProcessPtr process;
  if (idmef_process==NULL)
    return process;

  Persistency::Process::Path path;
  const prelude_string_t *idmef_path_str = idmef_process_get_path(idmef_process);

  if (idmef_path_str)
    path=prelude_string_get_string(idmef_path_str);

  prelude_string_t *idmef_process_str = idmef_process_get_name(idmef_process);
  if (idmef_process_str)
  {
    const Persistency::Process::Name  name=prelude_string_get_string(idmef_process_str);
    const uint32_t                   *pid =idmef_process_get_pid(idmef_process);
    const pid_t pidt=(pid==NULL)?0:*pid;;
    // pid is set to 0 if there is no pid in idmef
    process.reset(new Persistency::Process(path,name,NULL,&pidt,NULL,username,NULL,Persistency::ReferenceURLPtr()));
  }

  return process;
}

} // namespace Prelude
} // namespace Input


