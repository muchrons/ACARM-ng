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
#include "Trigger/Python/Strategy.hpp"
#include "Trigger/Python/Config.hpp"
#include "Trigger/Python/FactoryBuilder.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace Python
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_Python_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("python"),
  log_("trigger.python.factorybuilder")
{
}


FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig fc(type_.str(), options);

  // gather required config
  const Config::Path &path=fc["script"];
  LOGMSG_INFO_S(log_)<<"setting script path to "<<path;

  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // trigger name
  const std::string &name    =fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";

  // defile output type
  typedef InterfaceImpl<Python::Strategy, Python::Config> Impl;
  typedef FactoryBuilder::FactoryPtr                      OutPtr;
  // create and return new element
  return OutPtr( new Impl(type_, InstanceName(name), Python::Config(thCfg, path) ) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace Python
} // namespace Trigger
