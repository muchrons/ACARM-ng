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
#include "Trigger/GG/Config.hpp"
#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/FactoryBuilder.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace GG
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_GG_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("gg"),
  log_("trigger.gg.factorybuilder")
{
}


namespace
{
Config::Receivers parseReceivers(const Logger::Node &log, const string &str)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(" ");
  const Tokenizer tokens(str, sep);

  // go thought all receivers
  Tokenizer::const_iterator it=tokens.begin();
  if( it==tokens.end() )
    throw Exception(SYSTEM_SAVE_LOCATION, "no receivers specified");
  UserID uin=Commons::Convert::to<UserID>(*it);
  Config::Receivers r(uin);
  LOGMSG_INFO_S(log)<<"adding receiver's UIN "<<uin;
  ++it;

  // all other receivers are optional
  for(; it!=tokens.end(); ++it)
  {
    uin=Commons::Convert::to<UserID>(*it);
    r.push_back(uin);
    LOGMSG_INFO_S(log)<<"adding receiver's UIN "<<uin;
  }

  // return receivers list
  LOGMSG_DEBUG_S(log)<<"got total numer of "<< r.size() <<" receivers";
  return r;
} // parseReceivers()
} // unnamed namespace


FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig   fc(type_.str(), options);
  // account's config
  const UserID          uid =Commons::Convert::to<UserID>( fc["uin"] );
  LOGMSG_INFO_S(log_)<<"setting UIN to "<<uid;
  const std::string    &pass=fc["password"];
  LOGMSG_INFO_S(log_)<<"setting password ******** for UIN";
  const AccountConfig   account(uid, pass);
  // triggergg name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  // receivers' UINs
  const Config::Receivers receivers=parseReceivers(log_, fc["receivers"]);
  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // create and return new handle.
  typedef InterfaceImpl<GG::Strategy, GG::Config> Impl;
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_, InstanceName(name), GG::Config(account, receivers, thCfg) ) );
  return FactoryBuilder::FactoryPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace GG
} // namespace Trigger
