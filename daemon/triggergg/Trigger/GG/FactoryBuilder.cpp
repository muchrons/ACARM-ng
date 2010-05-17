/*
 * FactoryBuilder.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
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
  name_("gg"),
  log_("trigger.gg.factorybuilder")
{
}

namespace
{
UserID getID(const std::string &v)
{
  const long   tmp=boost::lexical_cast<long>(v);
  const UserID uid=boost::numeric_cast<UserID>(tmp);
  return uid;
} // getID()
} // unnamed namespace

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig   fc(name_, options);
  // account's config
  const UserID          uid =getID( fc["uin"] );
  LOGMSG_INFO_S(log_)<<"setting UIN to "<<uid;
  const std::string    &pass=fc["password"];
  LOGMSG_INFO_S(log_)<<"setting password ******** for UIN";
  const AccountConfig   account(uid, pass);
  // receiver's UID
  const UserID          receiverUid=getID( fc["receiver_uin"] );
  LOGMSG_INFO_S(log_)<<"setting receiver UIN to "<<receiverUid;
  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // create and return new handler.
  typedef InterfaceImpl<GG::Strategy, GG::Config> Impl;
  return FactoryBuilder::FactoryPtr( new Impl( name_, GG::Config(account, receiverUid, thCfg) ) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace GG
} // namespace Trigger
