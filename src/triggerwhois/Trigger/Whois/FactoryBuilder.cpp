/*
 * FactoryBuilder.cpp
 *
 */
#include <boost/filesystem.hpp>
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
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
  // thresholds' config
  const char   *sevTh =fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char   *cntTh =fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // create and return new handle.
  typedef InterfaceImpl<Whois::Strategy, Whois::Config> Impl;
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl( type_,
                                                    InstanceName(name),
                                                    Whois::Config(templatefile, thCfg) ) );
  return FactoryBuilder::FactoryPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace Whois
} // namespace Trigger
