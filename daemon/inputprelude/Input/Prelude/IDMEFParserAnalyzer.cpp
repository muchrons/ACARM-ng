/*
 * IDMEFParserAnalyzer.cpp
 *
 */
#include "ParseException.hpp"
#include "IDMEFParserAnalyzer.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;
using Persistency::Analyzer;

IDMEFParserAnalyzer::IDMEFParserAnalyzer(idmef_analyzer_t *ptr):
  name_(parseName(isEmpty(ptr))),
  version_(parseVersion(isEmpty(ptr))),
  os_(parseOs(isEmpty(ptr))),
  ip_(parseIP(isEmpty(ptr)))
{
}

idmef_analyzer_t * IDMEFParserAnalyzer::isEmpty(idmef_analyzer_t *ptr) const
{
  if (!ptr)
    throw ParseException(SYSTEM_SAVE_LOCATION, "Idmef Analyzer is empty.");
  return ptr;
}

Persistency::Analyzer::Name IDMEFParserAnalyzer::parseName(idmef_analyzer_t *ptr) const
{
  const prelude_string_t *idmef_name = idmef_analyzer_get_name(ptr);
  return Persistency::Analyzer::Name(prelude_string_get_string_or_default(idmef_name, "Unknown"));
}

std::auto_ptr<Persistency::Analyzer::Version> IDMEFParserAnalyzer::parseVersion(idmef_analyzer_t *ptr) const
{
  std::auto_ptr<Persistency::Analyzer::Version> ver;
  const prelude_string_t *idmef_version = idmef_analyzer_get_version(ptr );
  if (idmef_version)
    ver.reset(new Persistency::Analyzer::Version(prelude_string_get_string(idmef_version)));
  return ver;
}

std::auto_ptr<Persistency::Analyzer::OS> IDMEFParserAnalyzer::parseOs(idmef_analyzer_t *ptr) const
{
  std::auto_ptr<Persistency::Analyzer::OS> os;

  const prelude_string_t *idmef_ostype = idmef_analyzer_get_ostype(ptr);
  const prelude_string_t *idmef_osversion = idmef_analyzer_get_osversion(ptr);

  std::string osname="";

  if (idmef_ostype)
    osname=std::string(prelude_string_get_string_or_default(idmef_ostype, ""));

  if (idmef_osversion)
    osname+=std::string(prelude_string_get_string_or_default(idmef_osversion, ""));
  //final os version is a concatenation of the two

  if (osname!="")
    os.reset(new Persistency::Analyzer::OS(osname));
  return os;
}

std::auto_ptr<Persistency::Analyzer::IP> IDMEFParserAnalyzer::parseIP(idmef_analyzer_t *ptr) const
{
  std::auto_ptr<Persistency::Analyzer::IP> ip;
  idmef_node_t *idmef_node = idmef_analyzer_get_node(ptr);

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
            ip.reset(new Analyzer::IP(address_v4::from_string(tmp)));
            break;
          case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
          case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR_HEX:
          case IDMEF_ADDRESS_CATEGORY_IPV6_NET:
          case IDMEF_ADDRESS_CATEGORY_IPV6_NET_MASK:
            ip.reset(new Analyzer::IP(address_v6::from_string(tmp)));
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

const Persistency::Analyzer::Name& IDMEFParserAnalyzer::getName() const
{
  return name_;
}

const Persistency::Analyzer::Version* IDMEFParserAnalyzer::getVersion() const
{
  return version_.get();
}

const Persistency::Analyzer::OS* IDMEFParserAnalyzer::getOS() const
{
  return os_.get();
}

const Persistency::Analyzer::IP* IDMEFParserAnalyzer::getIP() const
{
  return ip_.get();
}

} // namespace Prelude
} // namespace Input


