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
#include "Trigger/SnortSam/Strategy.hpp"
#include "Trigger/SnortSam/Config.hpp"
#include "Trigger/SnortSam/FactoryBuilder.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;
using Trigger::Simple::ThresholdConfig;


namespace Trigger
{
namespace SnortSam
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Trigger_SnortSam_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("snortsam"),
  log_("trigger.snortsam.factorybuilder")
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

Who getWho(const std::string &v)
{
  if(v=="src")
    return Who::SRC;
  if(v=="dst")
    return Who::DST;
  // ok - we have invalid value here...
  throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "who", v.c_str() );
} // getWho()

How getHow(const std::string &v)
{
  if(v=="in")
    return How::IN;
  if(v=="out")
    return How::OUT;
  // ok - we have invalid value here...
  throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "how", v.c_str() );
} // getHow()


template<typename T>
T getParsedFiled(const std::string &line, T (*getter)(const std::string &) )
{
  assert(getter!=NULL);
  // prepare tokenizer
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(" ");
  const Tokenizer tokens(line, sep);

  // sanity check
  Tokenizer::const_iterator it=tokens.begin();
  if( it==tokens.end() )
    throw Exception(SYSTEM_SAVE_LOCATION, "no elements specified");
  // go thought all tokens
  T out;
  for(; it!=tokens.end(); ++it)
  {
    const T tmp=(*getter)(*it);
    out=out|tmp;
    LOGMSG_INFO_S(log)<<"adding option: "<<tmp.toInt();
  }

  // return final result
  return out;
} // getParsedFiled()
} // unnamed namespace


FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig fc(type_.str(), options);

  // gather required config
  const std::string &host=fc["host"];
  LOGMSG_INFO_S(log_)<<"setting host to '"<<host<<"'";
  const uint16_t     port=getPort(fc["port"]);
  LOGMSG_INFO_S(log_)<<"setting port to "<<port;
  const std::string &key=fc["key"];
  LOGMSG_INFO(log_, "setting key to ******");
  // TODO
  /*
  const Who          who=getParsedField<Who>(fc["who"], getWho);
  LOGMSG_INFO_S(log_)<<"setting 'who' to "<<who.toInt();
  const How          how=getParsedField<How>(fc["how"], getHow);
  LOGMSG_INFO_S(log_)<<"setting 'how' to "<<how.toInt();
  */
  const unsigned int duration=Commons::Convert::to<unsigned int>(fc["duration"]);
  LOGMSG_INFO_S(log_)<<"setting duration to "<<duration<<"[s]";

  // thresholds' config
  const char *sevTh=fc.get("severity_threshold");
  if(sevTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting severity threshold to "<<sevTh;
  const char *cntTh=fc.get("alerts_count_threshold");
  if(cntTh!=NULL)
    LOGMSG_INFO_S(log_)<<"setting alerts count threshold to "<<cntTh;
  const ThresholdConfig thCfg(sevTh, cntTh);

  // trigger name
  const std::string &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting trigger \""<<getTypeName()<<"\" name to \""<<name<<"\"";

  // define output type
  typedef InterfaceImpl<SnortSam::Strategy, SnortSam::Strategy::Parameters> Impl;
  typedef FactoryBuilder::FactoryPtr                                        OutPtr;

  // create and return new handle
  const SnortSam::Config              cfg(host, port, key, who, how, duration, thCfg);
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl(type_, InstanceName(name), cfg_) );
  return OutPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace SnortSam
} // namespace Trigger
