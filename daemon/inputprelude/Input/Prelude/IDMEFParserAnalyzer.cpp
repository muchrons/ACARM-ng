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

  const prelude_string_t *idmef_name = idmef_analyzer_get_name(ptr_);
  name_ = prelude_string_get_string_or_default(idmef_name, "Unknown");

  const prelude_string_t *idmef_version = idmef_analyzer_get_version(ptr_);
  if (idmef_version)
    version_.reset(new Persistency::Analyzer::Version(prelude_string_get_string(idmef_version)));

  const prelude_string_t *idmef_ostype = idmef_analyzer_get_ostype(ptr_);
  const prelude_string_t *idmef_osversion = idmef_analyzer_get_osversion(ptr_);

  std::string osname="";

  if (idmef_ostype)
    osname=std::string(prelude_string_get_string_or_default(idmef_ostype, ""));

  if (idmef_osversion)
    osname+=std::string(prelude_string_get_string_or_default(idmef_osversion, ""));

  if (osname!="")
    os_.reset(new Persistency::Analyzer::Name(osname));

  idmef_node_t *idmef_node = idmef_analyzer_get_node(ptr_);

  if (idmef_node)
    {
      idmef_address_t *idmef_node_addr = idmef_node_get_next_address(idmef_node, NULL);
      if (idmef_node_addr)
        {
          const prelude_string_t *idmef_node_address = idmef_address_get_address(idmef_node_addr);
          if (idmef_node_address)
            {
              switch (idmef_address_get_category(idmef_node_addr))
                {
                IDMEF_ADDRESS_CATEGORY_IPV4_ADDR:
                IDMEF_ADDRESS_CATEGORY_IPV4_ADDR_HEX: //<-- What does it look like? Does it work with asio? I dunno. Gotta check.
                IDMEF_ADDRESS_CATEGORY_IPV4_NET:
                IDMEF_ADDRESS_CATEGORY_IPV4_NET_MASK:
                  ip_.reset(new Persistency::Analyzer::IP(boost::asio::ip::address_v4::from_string(prelude_string_get_string(idmef_node_address))));
                  break;
                IDMEF_ADDRESS_CATEGORY_IPV6_ADDR:
                IDMEF_ADDRESS_CATEGORY_IPV6_ADDR_HEX:
                IDMEF_ADDRESS_CATEGORY_IPV6_NET:
                IDMEF_ADDRESS_CATEGORY_IPV6_NET_MASK:
                  ip_.reset(new Persistency::Analyzer::IP(boost::asio::ip::address_v6::from_string(prelude_string_get_string(idmef_node_address))));
                  break;
                default:
                  break;
                }
            }
        }
    }
}

Persistency::Analyzer::Name IDMEFParserAnalyzer::getName() const
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


