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
#include "Trigger/GG/FactoryBuilder.hpp"
#include "Trigger/GG/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


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

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building trigger's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const TriggerConfig fc(name_, options);
  /*
  const int           timeoutInt=boost::lexical_cast<int>( fc["timeout"] );
  const unsigned int  timeout   =boost::numeric_cast<unsigned int>(timeoutInt);
  */
  //LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";

  // create and return new handler.
  typedef InterfaceImpl<Strategy, unsigned int> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(name_, 666) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace GG
} // namespace Trigger
