/*
 * IDMEFParserAnalyzer.cpp
 *
 */
#include "Input/Prelude/ParseException.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;
using Persistency::Analyzer;

IDMEFParserAnalyzer::IDMEFParserAnalyzer(idmef_analyzer_t *ptr):
  name_(parseName(getNonNull(ptr))),
  version_(parseVersion(getNonNull(ptr))),
  os_(parseOS(getNonNull(ptr))),
  ip_(parseIP(getNonNull(ptr)))
{
}

idmef_analyzer_t * IDMEFParserAnalyzer::getNonNull(idmef_analyzer_t *ptr) const
{
  if(ptr==NULL)
    throw ParseException(SYSTEM_SAVE_LOCATION, "Idmef Analyzer is empty.");
  return ptr;
}

Persistency::Analyzer::Name IDMEFParserAnalyzer::parseName(idmef_analyzer_t *ptr) const
{
  // TODO: c&p code
  const prelude_string_t *idmef_name = idmef_analyzer_get_name(ptr);
  // TODO: throw on error
  if (idmef_name)
    return prelude_string_get_string(idmef_name);
  return "Unknown";
}

std::auto_ptr<Persistency::Analyzer::Version> IDMEFParserAnalyzer::parseVersion(idmef_analyzer_t *ptr) const
{
  std::auto_ptr<Persistency::Analyzer::Version> ver;
  const prelude_string_t *idmef_version = idmef_analyzer_get_version(ptr );

  if (idmef_version!=NULL)
    ver.reset(new Persistency::Analyzer::Version(prelude_string_get_string(idmef_version)));
  return ver;
}

std::auto_ptr<Persistency::Analyzer::OS> IDMEFParserAnalyzer::parseOS(idmef_analyzer_t *ptr) const
{
  // TODO: consider using NullValue<> here
  std::auto_ptr<Persistency::Analyzer::OS> os;

  const prelude_string_t *idmef_ostype = idmef_analyzer_get_ostype(ptr);
  const prelude_string_t *idmef_osversion = idmef_analyzer_get_osversion(ptr);

  // TODO: can't prelude_string_get_string_or_default() be used directly here?
  std::string osname="";

  if (idmef_ostype!=NULL)
    osname=std::string(prelude_string_get_string_or_default(idmef_ostype, ""));

  if (idmef_osversion!=NULL)
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

  if (idmef_node==NULL)
    return ip;

  idmef_address_t *idmef_node_addr = idmef_node_get_next_address(idmef_node, NULL);
  if (idmef_node_addr==NULL)
    return ip;

  const prelude_string_t *idmef_node_address = idmef_address_get_address(idmef_node_addr);
  if (idmef_node_address==NULL)
    return ip;

  const char * tmp=prelude_string_get_string(idmef_node_address);
  switch (idmef_address_get_category(idmef_node_addr))
    {
    case IDMEF_ADDRESS_CATEGORY_IPV4_ADDR:
    case IDMEF_ADDRESS_CATEGORY_IPV4_NET:
      ip.reset(new Analyzer::IP(address_v4::from_string(tmp)));
      break;
    case IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
    case IDMEF_ADDRESS_CATEGORY_IPV6_NET:
      ip.reset(new Analyzer::IP(address_v6::from_string(tmp)));
      break;
    default:
      assert(!"Unknown type of IP address, sorry...");
      break;
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


