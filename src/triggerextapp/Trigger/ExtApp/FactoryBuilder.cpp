/*
 * FactoryBuilder.cpp
 *
 */
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Trigger/ExtApp/Config.hpp"
#include "Trigger/ExtApp/Strategy.hpp"
#include "Trigger/ExtApp/FactoryBuilder.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace ExtApp
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_ExtApp_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("extapp"),
  log_("trigger.extapp.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig  fc(type_.str(), options);
  // path to executable
  const Config::Path  &path=fc["executable"];
  LOGMSG_INFO_S(log_)<<"setting application to run to: "<<path;
  // triggergg name
  const std::string   &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // create and return new handle.
  typedef InterfaceImpl<ExtApp::Strategy, ExtApp::Config> Impl;
  return FactoryBuilder::FactoryPtr( new Impl( type_, InstanceName(name), ExtApp::Config(path, thCfg) ) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace ExtApp
} // namespace Trigger
