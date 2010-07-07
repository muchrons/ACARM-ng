/*
 * IDMEFParserAnalyzer.cpp
 *
 */
#include <sstream>

#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserCommons.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"

using namespace std;

namespace Input
{
namespace Prelude
{

using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;
using Persistency::Analyzer;

IDMEFParserAnalyzer::IDMEFParserAnalyzer(idmef_analyzer_t *ptr):
  log_("input.prelude.ipa"),
  name_( parseName( getNonNull(ptr) ) ),
  version_( parseVersion( getNonNull(ptr) ) ),
  os_( parseOS( getNonNull(ptr) ) ),
  ip_( parseIP( getNonNull(ptr) ) ),
  preludeID_( parsePreludeID( getNonNull(ptr) ) )
{
}

idmef_analyzer_t * IDMEFParserAnalyzer::getNonNull(idmef_analyzer_t *ptr) const
{
  if(ptr==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Idmef Analyzer is empty.");
  return ptr;
}

namespace
{
const char *nonNULL(const char *str)
{
  if(str==NULL)
    return "NULLSTR";
  return str;
} // nonNULL()
string nonNULL(const Persistency::Analyzer::IP *ip)
{
  if(ip==NULL)
    return "NULLIP";
  return ip->to_string();
} // nonNULL()
} // unnamed namespace

std::string IDMEFParserAnalyzer::parsePreludeID(idmef_analyzer_t *ptr) const
{
  const prelude_string_t *idmef_id = idmef_analyzer_get_analyzerid(ptr);
  if(idmef_id==NULL)
  {
    // if id's not set directly, create some string out of what we have...
    stringstream ss;
    ss << "UnknownID/" << nonNULL( getName().get() )
       << "/" << nonNULL( getVersion().get() )
       << "/" << nonNULL( getOS().get() )
       << "/" << nonNULL( getIP() );
    return ss.str();
  }
  return prelude_string_get_string(idmef_id);
}

Persistency::Analyzer::Name IDMEFParserAnalyzer::parseName(idmef_analyzer_t *ptr) const
{
  const prelude_string_t *idmef_name = idmef_analyzer_get_name(ptr);
  if (idmef_name)
    return prelude_string_get_string(idmef_name);
  return "Unknown";
}

Persistency::Analyzer::Version IDMEFParserAnalyzer::parseVersion(idmef_analyzer_t *ptr) const
{
  Persistency::Analyzer::Version ver(NULL);
  const prelude_string_t *idmef_version = idmef_analyzer_get_version(ptr );

  if (idmef_version!=NULL)
    ver=Persistency::Analyzer::Version(prelude_string_get_string(idmef_version));
  return ver;
}

Persistency::Analyzer::OperatingSystem IDMEFParserAnalyzer::parseOS(idmef_analyzer_t *ptr) const
{
  Persistency::Analyzer::OperatingSystem os(NULL);

  const prelude_string_t *idmef_ostype = idmef_analyzer_get_ostype(ptr);
  const prelude_string_t *idmef_osversion = idmef_analyzer_get_osversion(ptr);

  std::string osname;

  if (idmef_ostype!=NULL)
    osname=std::string(prelude_string_get_string_or_default(idmef_ostype, ""));

  if (idmef_osversion!=NULL)
    osname+=std::string(prelude_string_get_string_or_default(idmef_osversion, ""));
  //final os version is a concatenation of the two

  if (osname!="")
    os=Persistency::Analyzer::OperatingSystem(osname);
  return os;
}

std::auto_ptr<Persistency::Analyzer::IP> IDMEFParserAnalyzer::parseIP(idmef_analyzer_t *ptr) const
{
  // TODO: consider using Base::NullValue instead of auto_ptr<> here.
  std::auto_ptr<Persistency::Analyzer::IP> ip;
  idmef_node_t *idmef_node = idmef_analyzer_get_node(ptr);

  try
  {
    ip.reset(new Analyzer::IP(IDMEFParserCommons::getIPfromIdmefNode(idmef_node)));
  }
  catch(const ExceptionParse &)
  {
    LOGMSG_WARN(log_, "No IP but we can carry on");
  }
  return ip;
}

const std::string &IDMEFParserAnalyzer::getPreludeID() const
{
  return preludeID_;
}

const Persistency::Analyzer::Name& IDMEFParserAnalyzer::getName() const
{
  return name_;
}

const Persistency::Analyzer::Version &IDMEFParserAnalyzer::getVersion() const
{
  return version_;
}

const Persistency::Analyzer::OperatingSystem &IDMEFParserAnalyzer::getOS() const
{
  return os_;
}

const Persistency::Analyzer::IP* IDMEFParserAnalyzer::getIP() const
{
  return ip_.get();
}

} // namespace Prelude
} // namespace Input


