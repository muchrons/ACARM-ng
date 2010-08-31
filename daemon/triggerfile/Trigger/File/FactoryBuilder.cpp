/*
 * FactoryBuilder.cpp
 *
 */
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Trigger/File/Config.hpp"
#include "Trigger/File/Strategy.hpp"
#include "Trigger/File/FactoryBuilder.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace File
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_File_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("file"),
  log_("trigger.file.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig   fc(type_, options);
  // output direcotry
  const string &outdir=fc["outdir"];
  if( outdir.length()==0 )
    throw ExceptionInvalidDirectory(SYSTEM_SAVE_LOCATION, outdir);
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
  typedef InterfaceImpl<File::Strategy, File::Config> Impl;
  return FactoryBuilder::FactoryPtr( new Impl( type_, name, File::Config(outdir, thCfg) ) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace File
} // namespace Trigger
