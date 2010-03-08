/*
 * FactoryBuilder.cpp
 *
 */
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/FilterConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Filter/DNSResolver/FactoryBuilder.hpp"
#include "Filter/DNSResolver/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace DNSResolver
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_DNSResolver_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  name_("dnsresolver"),
  log_("filter.dnsresolver.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig         fc(name_, options);
  const Strategy::Parameters params=Strategy::Parameters();
  //LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";

  // create and return new handler.
  typedef InterfaceImpl<Strategy, Strategy::Parameters> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(name_, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace DNSResolver
} // namespace Filter
