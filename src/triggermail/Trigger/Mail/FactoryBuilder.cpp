/*
 * FactoryBuilder.cpp
 *
 */
#include <boost/tokenizer.hpp>
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "Commons/Convert.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Trigger/Mail/Strategy.hpp"
#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/FactoryBuilder.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace Mail
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_Mail_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("mail"),
  log_("trigger.mail.factorybuilder")
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

::Mail::Config::Recipients parseRecipients(const Logger::Node &log, const string &str)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(" ");
  const Tokenizer tokens(str, sep);

  // go thought all receivers
  Tokenizer::const_iterator it=tokens.begin();
  if( it==tokens.end() )
    throw Exception(SYSTEM_SAVE_LOCATION, "no receivers specified");
  ::Mail::Config::Recipients r(*it);
  LOGMSG_INFO_S(log)<<"adding receiver's e-mail "<<*it;
  ++it;

  // all other receivers are optional
  for(; it!=tokens.end(); ++it)
  {
    r.push_back(*it);
    LOGMSG_INFO_S(log)<<"adding receiver's e-mail "<<*it;
  }

  // return inal list
  LOGMSG_DEBUG_S(log)<<"got total numer of "<< r.size() <<" receivers";
  return r;
} // parseRecipients()

} // unnamed namespace


FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig fc(type_.str(), options);

  // gather required config
  const std::string                            &server=fc["server"];
  LOGMSG_INFO_S(log_)<<"setting server to "<<server;
  const ::Mail::Config::Path             cert  =fc["rootca"];
  LOGMSG_INFO_S(log_)<<"setting root's CA certificate path to "<<cert;
  const uint16_t                               port  =getPort(fc["port"]);
  LOGMSG_INFO_S(log_)<<"setting port to "<<port;
  const ::Mail::Config::Server::Security sec   =getSecurity(fc["security"]);
  LOGMSG_INFO_S(log_)<<"setting security to "<<sec.toInt();
  const ::Mail::Config::Server::Protocol proto =getProtocol(fc["protocol"]);
  LOGMSG_INFO_S(log_)<<"setting protocol to "<<proto.toInt();
  const ::Mail::Config::Server           serverCfg(server, port, proto, sec, cert);

  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // sender's address
  const std::string        &from=fc["from"];
  LOGMSG_INFO_S(log_)<<"setting sender's address to "<<from;
  // recipient address
  const ::Mail::Config::Recipients  to  =parseRecipients(log_, fc["to"]);

  // trigger name
  const std::string &name    =fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";

  // defile output type
  typedef InterfaceImpl<Mail::Strategy, Mail::Config> Impl;
  typedef FactoryBuilder::FactoryPtr                  OutPtr;

  // check for authorization requirement
  const char *user=fc.get("user");

  if(user!=NULL)
  {
    LOGMSG_INFO_S(log_)<<"setting user to "<<user;
    const std::string           &pass=fc["password"];
    const ::Mail::Config::Authorization  auth(user, pass);
    const ::Mail::Config mailCfg(from, serverCfg, auth);
    // create and return new handle, with configured authorization
    LOGMSG_INFO(log_, "account configured with authorization required");
    InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_, InstanceName(name), Mail::Config(thCfg, mailCfg, to) ) );
    return OutPtr(new InterfaceWrapper(ptr));
  } // if(use_auth)
  else
    if( fc.get("password")!=NULL )
      LOGMSG_WARN(log_, "user not configured, but password provided - skipping any way");

  const ::Mail::Config mailCfg(from, serverCfg);
  // create and return new handle, with config without authorization
  LOGMSG_INFO(log_, "account configured without authorization required");
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_, InstanceName(name), Mail::Config(thCfg, mailCfg, to) ) );
  return OutPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace Mail
} // namespace Trigger
