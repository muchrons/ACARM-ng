/*
 * IDMEFParserAnalyzer.cpp
 *
 */
#include "Input/Exception.hpp"
#include "IDMEFParserAnalyzer.hpp"


namespace Input
{
namespace Prelude
{

IDMEFParserAnalyzer::IDMEFParserAnalyzer(idmef_analyzer_t *ptr):ptr_(ptr)
{
  if (!ptr)
    throw Exception(SYSTEM_SAVE_LOCATION, "Idmef Analyzer is empty.");
}
 
  //todo what about analyzer ID, manufacturer and model ?
Persistency::Analyzer::Name IDMEFParserAnalyzer::getName() const
{
  const prelude_string_t *field = idmef_analyzer_get_name(ptr_);
  return Persistency::Analyzer::Name(prelude_string_get_string_or_default(field, "Unknown"));
}

Persistency::Analyzer::Version IDMEFParserAnalyzer::getVersion() const
{
  const prelude_string_t *idmef_version = idmef_analyzer_get_version(ptr_);
  return Persistency::Analyzer::Version(prelude_string_get_string_or_default(idmef_version, ""));
}
  
Persistency::Analyzer::OS IDMEFParserAnalyzer::getOS() const
{
  const prelude_string_t *idmef_ostype = idmef_analyzer_get_ostype(ptr_);
  const prelude_string_t *idmef_osversion = idmef_analyzer_get_osversion(ptr_);
  return std::string(prelude_string_get_string_or_default(idmef_ostype, ""))+std::string(prelude_string_get_string_or_default(idmef_osversion, ""));
}

Persistency::Analyzer::IP IDMEFParserAnalyzer::getIP() const
{
  idmef_node_t *field = idmef_analyzer_get_node(ptr);
  
  return boost::asio::ip::address_v6::from_string(prelude_string_get_string_or_default(address, ""));
}
  /*
Persistency::HostPtr IDMEFParserAnalyzer::getHost() const
{
  const prelude_string_t *ostype = idmef_analyzer_get_ostype(ptr_);
  const prelude_string_t *osversion = idmef_analyzer_get_osversion(ptr_);  
  
  idmef_node_t *node= idmef_analyzer_get_node(ptr_);
  if(node)
    {
      idmef_address_t *elem = NULL;
      
      bool ipv6=false;
      prelude_string_t *address=NULL,*netmask=NULL;
      
      while ( (elem = idmef_node_get_next_address(node, elem)) )
	{      
	  int i = idmef_address_get_category(elem);
	  switch(i)
	    {
	    case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR:
	    case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR_HEX:
	      address = idmef_address_get_address(elem);
	      break;
	      
	    case IDMEF_ADDRESS_CATEGORY_IPV4_NET:
	    case IDMEF_ADDRESS_CATEGORY_IPV4_NET_MASK:
	      netmask = idmef_address_get_netmask(elem);
	      break;
	      
	    case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
	    case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR_HEX:
	      ipv6=true;
	      address = idmef_address_get_address(elem);	  
	      break;
	      
	    case IDMEF_ADDRESS_CATEGORY_IPV6_NET:
	    case IDMEF_ADDRESS_CATEGORY_IPV6_NET_MASK:
	      ipv6=true;
	      netmask = idmef_address_get_netmask(elem);	  
	      break;
	    }
	}
      
      Persistency::Host::OperatingSystem os = std::string(prelude_string_get_string_or_default(ostype, "unknown")) + std::string(prelude_string_get_string_or_default(osversion, ""));
      Persistency::Host::ReportedProcesses rp;
      Persistency::Host::ReportedServices rs;

      idmef_process_t *process = idmef_analyzer_get_process(ptr_);
      if (process)
	{
	  prelude_string_t *idmef_name = idmef_process_get_name(process);
	  prelude_string_t *idmef_path = idmef_process_get_path(process);
	  uint32_t *idmef_pid = idmef_process_get_pid(process);
	  pid_t pid=static_cast<pid_t>(*idmef_pid);
	  Persistency::Process::Name name(prelude_string_get_string_or_default(idmef_name, "Unknown"));
	  Persistency::Process::Path path(prelude_string_get_string_or_default(idmef_path, ""));	  
	  rp.push_back(Persistency::ProcessPtrNN(new Persistency::Process(path,name,NULL,&pid,NULL,NULL,NULL,Persistency::ReferenceURLPtr())));
	}

      if(ipv6)
	{
	  Persistency::Host::IPv6 ip(boost::asio::ip::address_v6::from_string(prelude_string_get_string_or_default(address, "::")));
	  if (netmask) 
	    {
	      Persistency::Host::Netmask_v6 nm(boost::asio::ip::address_v6::from_string(prelude_string_get_string_or_default(address, "")));
	      return Persistency::HostPtr(new Persistency::Host(ip,&nm,os,Persistency::ReferenceURLPtr(),rs,rp));
	    } else
	    return Persistency::HostPtr(new Persistency::Host(ip,NULL,os,Persistency::ReferenceURLPtr(),rs,rp));
	} else
	{
	  Persistency::Host::IPv4 ip(boost::asio::ip::address_v4::from_string(prelude_string_get_string_or_default(address, "0.0.0.0")));
	  if (netmask) 
	    {
	      Persistency::Host::IPv4 nm(boost::asio::ip::address_v4::from_string(prelude_string_get_string_or_default(address, "0.0.0.0")));
	      return Persistency::HostPtr(new Persistency::Host(ip,&nm,os,Persistency::ReferenceURLPtr(),rs,rp));
	    } else
	    return Persistency::HostPtr(new Persistency::Host(ip,NULL,os,Persistency::ReferenceURLPtr(),rs,rp));
	}
    }
  return Persistency::HostPtr();
}
  */
} // namespace Prelude
} // namespace Input


