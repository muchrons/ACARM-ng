/*
 * FactoryBuilder.cpp
 *
 */
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "Commons/Convert.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Mail/Config.hpp"
#include "Trigger/Whois/Config.hpp"
#include "Trigger/Whois/Strategy.hpp"
#include "Trigger/Whois/FactoryBuilder.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace Whois
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_Whois_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("whois"),
  log_("trigger.whois.factorybuilder")
{
}

namespace
{
uint16_t getPort(const std::string &v)
{
  try
  {
    return Commons::Convert::to<uint16_t>(v);;
  }
  catch(const std::exception &ex)
  {
    // re-throw common type
    throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "port", v.c_str(), ex.what() );
  }
} // getPort()

::Mail::Config::Server::Protocol getProtocol(const std::string &v)
{
  if(v=="smtp")
    return ::Mail::Config::Server::Protocol::SMTP;

  // ok - we have invalid value here...
  throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "protocol", v.c_str() );
} // getSecurity()

::Mail::Config::Server::Security getSecurity(const std::string &v)
{
  if(v=="ssl")
    return ::Mail::Config::Server::Security::SSL;
  if(v=="starttls")
    return ::Mail::Config::Server::Security::TLS;

  // ok - we have invalid value here...
  throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "security", v.c_str() );
} // getSecurity()

::Mail::Config::copyRecipients parseCopyRecipients(const Logger::Node &log, const string &str)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(" ");
  const Tokenizer tokens(str, sep);

  // go thought all receivers
  Tokenizer::const_iterator it=tokens.begin();
  if( it==tokens.end() )
    throw Exception(SYSTEM_SAVE_LOCATION, "no receivers specified");
  ::Mail::Config::copyRecipients r;
  LOGMSG_INFO_S(log)<<"adding receiver's e-mail "<<*it;

  // all other receivers are optional
  for(; it!=tokens.end(); ++it)
  {
    r.push_back(*it);
    LOGMSG_INFO_S(log)<<"adding receiver's e-mail "<<*it;
  }

  // return final list
  LOGMSG_DEBUG_S(log)<<"got total numer of "<< r.size() <<" receivers";
  return r;
} // parseRecipients()

} // unnamed namespace

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig fc(type_.str(), options);
  // file with template message
  const path &templatefile=fc["template"];
  if( Commons::Filesystem::isFileSane(templatefile)==false )
  {
    LOGMSG_FATAL_S(log_)<<"message template file '"<<templatefile<<"' does not exist";
    throw ExceptionInvalidFile(SYSTEM_SAVE_LOCATION, templatefile);
  }
  // triggerfile name
  const string &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";

  // mail server config
  const std::string                      &server=fc["server"];
  LOGMSG_INFO_S(log_)<<"setting server to "<<server;
  const ::Mail::Config::Path             cert=fc["rootca"];
  LOGMSG_INFO_S(log_)<<"setting root's CA certificate path to "<<cert;
  const uint16_t                         port=getPort(fc["port"]);
  LOGMSG_INFO_S(log_)<<"setting port to "<<port;
  const ::Mail::Config::Server::Security sec=getSecurity(fc["security"]);
  LOGMSG_INFO_S(log_)<<"setting security to "<<sec.toInt();
  const ::Mail::Config::Server::Protocol proto=getProtocol(fc["protocol"]);
  LOGMSG_INFO_S(log_)<<"setting protocol to "<<proto.toInt();
  const ::Mail::Config::Server           serverCfg(server, port, proto, sec, cert);

  // sender's address
  const std::string                     &from=fc["from"];
  LOGMSG_INFO_S(log_)<<"setting sender's address to "<<from;
  // recipient address
  // TODO: shoulld be obtained from WHOIS database
  const ::Mail::Config::Recipients      to("admin@x.com");

  // thresholds' config
  const char   *sevTh =fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char   *cntTh =fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // copy recipient adress
  ::Mail::Config::copyRecipients cc;
  // check for copy recipients requirement
  const char *ccRecipients=fc.get("cc");
  if(ccRecipients != NULL)
    cc=parseCopyRecipients(log_, fc["cc"]);

  // defile output type
  typedef InterfaceImpl<Whois::Strategy, Whois::Config> Impl;
  typedef FactoryBuilder::FactoryPtr                  OutPtr;

  // check for authorization requirement
  const char *user=fc.get("user");

  if(user!=NULL)
  {
    LOGMSG_INFO_S(log_)<<"setting user to "<<user;
    const std::string                   &pass=fc["password"];
    const ::Mail::Config::Authorization auth(user, pass);
    const ::Mail::Config                mailCfg(from, to, serverCfg, auth, cc);
    // create and return new handle, with configured authorization
    LOGMSG_INFO(log_, "account configured with authorization required");
    InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_,
                                                      InstanceName(name),
                                                      Whois::Config(templatefile, thCfg, mailCfg) ) );
    return OutPtr(new InterfaceWrapper(ptr));
  } // if(use_auth)
  else
    if( fc.get("password")!=NULL )
      LOGMSG_WARN(log_, "user not configured, but password provided - skipping any way");

  const ::Mail::Config mailCfg(from, to, serverCfg, cc);
  // create and return new handle, with config without authorization
  LOGMSG_INFO(log_, "account configured without authorization required");
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_,
                                                    InstanceName(name),
                                                    Whois::Config(templatefile, thCfg, mailCfg) ) );
  return OutPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace Whois
} // namespace Trigger
