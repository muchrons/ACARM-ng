/*
 * FactoryBuilder.cpp
 *
 */
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/FilterConfig.hpp"
#include "Commons/Convert.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Filter/IPBlackList/FactoryBuilder.hpp"
#include "Filter/IPBlackList/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace IPBlackList
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_IPBlackList_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("ipblacklist"),
  log_("filter.ipblacklist.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_.str(), options);
  // filter ipblacklist name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";;
  const unsigned int refresh =Commons::Convert::to<unsigned int>( fc["refresh"] );
  const unsigned int limit   =Commons::Convert::to<unsigned int>( fc["limit"] );
  const double       priDelta=Commons::Convert::to<double>( fc["priorityDelta"] );
  LOGMSG_INFO_S(log_)<<"setting refresh interval to "<<refresh<<"[s]";
  LOGMSG_INFO_S(log_)<<"setting entries limit to "<<limit;
  Strategy::Parameters params(refresh, limit, priDelta);

  // create and return new handle.
  typedef InterfaceImpl<Strategy, Strategy::Parameters> Impl;
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl(type_, InstanceName(name), params) );
  return FactoryBuilder::FactoryPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace IPBlackList
} // namespace Filter
