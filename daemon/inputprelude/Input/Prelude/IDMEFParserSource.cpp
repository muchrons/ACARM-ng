/*
 * IDMEFParserSource.cpp
 *
 */
#include "Input/Prelude/ParseException.hpp"
#include "Input/Prelude/IDMEFParserSource.hpp"
#include "Persistency/Process.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;


IDMEFParserSource::IDMEFParserSource(idmef_source_t *ptr):
  ip_(parseIP(isEmpty(ptr))),
  process_(parseProcess(isEmpty(ptr))),
  service_(parseService(isEmpty(ptr)))
{
}

idmef_source_t * IDMEFParserSource::isEmpty(idmef_source_t *ptr) const
{
  if (!ptr)
    throw ParseException(SYSTEM_SAVE_LOCATION, "Idmef Source is empty.");
  return ptr;
}

std::auto_ptr<IDMEFParserSource::IP> IDMEFParserSource::parseIP(idmef_source_t *ptr) const
{
  // TODO: c&p from IDMEFParserAnalyzer::parseIP - make this a common code


  idmef_node_t *idmef_node = idmef_source_get_node(ptr);
  
  std::auto_ptr<IP> ip;
  if (idmef_node)
  {
    idmef_address_t *idmef_node_addr = idmef_node_get_next_address(idmef_node, NULL);
    if (idmef_node_addr)
    {
      const prelude_string_t *idmef_node_address = idmef_address_get_address(idmef_node_addr);
      if (idmef_node_address)
      {
        const char * tmp=prelude_string_get_string(idmef_node_address);
        switch (idmef_address_get_category(idmef_node_addr))
        {
          case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR:
          case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR_HEX:
          case IDMEF_ADDRESS_CATEGORY_IPV4_NET:
          case IDMEF_ADDRESS_CATEGORY_IPV4_NET_MASK:
            ip.reset(new IP(address_v4::from_string(tmp)));
            break;
          case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
          case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR_HEX:
          case IDMEF_ADDRESS_CATEGORY_IPV6_NET:
          case IDMEF_ADDRESS_CATEGORY_IPV6_NET_MASK:
            ip.reset(new IP(address_v6::from_string(tmp)));
            break;
          default:
            assert(!"Unknown type of IP address, skipping...");
            break;
        }
      }
    }
  }
  return ip;
}

Persistency::ProcessPtr IDMEFParserSource::parseProcess(idmef_source_t * ptr) const
{
  Persistency::Process::Username username="";

  idmef_user_t *idmef_user = idmef_source_get_user(ptr);
  if (idmef_user)
  {
    idmef_user_id_t *idmef_user_id = idmef_user_get_next_user_id(idmef_user, NULL);
    if (idmef_user_id)
    {
        prelude_string_t *user_id_str = idmef_user_id_get_name(idmef_user_id);
        if (user_id_str)
          username=prelude_string_get_string(user_id_str);
    }
  }

  // TODO: what if process is NULL?
  Persistency::ProcessPtr process;
  idmef_process_t *idmef_process = idmef_source_get_process(ptr);
  if (idmef_process)
  {
    // TODO: what is path is NULL?
    Persistency::Process::Path path;
    const prelude_string_t *idmef_path_str = idmef_process_get_path(idmef_process);

     if (idmef_path_str)
       path=prelude_string_get_string(idmef_path_str);

      prelude_string_t *idmef_process_str = idmef_process_get_name(idmef_process);
      if (idmef_process_str)
      {
        const Persistency::Process::Name name=prelude_string_get_string(idmef_process_str);
        const uint32_t *pid=idmef_process_get_pid(idmef_process);
        const pid_t pidt=*pid;
        process.reset(new Persistency::Process(path,name,NULL,&pidt,NULL,username,NULL,Persistency::ReferenceURLPtr()));
      }
  }
  return process;
}

Persistency::ServicePtr IDMEFParserSource::parseService(idmef_source_t * ptr) const
{
  Persistency::ServicePtr service;
  idmef_service_t *idmef_service = idmef_source_get_service(ptr);
  if (idmef_service)
  {
    prelude_string_t *idmef_service_name = idmef_service_get_name(idmef_service);
    if (idmef_service_name)
      {
        const Persistency::Service::Name name=prelude_string_get_string(idmef_service_name);
        // TODO: what if protocol is NULL?
        Persistency::Service::Protocol protocol;
        const prelude_string_t *idmef_protocol_str = idmef_service_get_iana_protocol_name(idmef_service);
        if (idmef_protocol_str)
          protocol=prelude_string_get_string(idmef_protocol_str);

        const uint16_t *idmef_port = idmef_service_get_port(idmef_service);

          // TODO: segv. when idmef_port is NULL
          // NOTE: read idmef_service_get_port() doc and use 'if' or 'assert' statement, according to needs.
        service.reset(new Persistency::Service(name,*idmef_port,protocol,Persistency::ReferenceURLPtr()));
      }
  }
  return service;
}

Persistency::Analyzer::IP IDMEFParserSource::getAddress() const
{
  // TODO: segv. when ip_==NULL
  return *ip_;
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


