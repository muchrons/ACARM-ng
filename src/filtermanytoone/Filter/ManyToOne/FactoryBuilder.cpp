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
#include "Filter/ManyToOne/FactoryBuilder.hpp"
#include "Filter/ManyToOne/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace ManyToOne
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_ManyToOne_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("manytoone"),
  log_("filter.manytoone.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_.str(), options);
  // filter manytoone name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  const unsigned int timeout=Commons::Convert::to<unsigned int>(fc["timeout"]);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";

  // create and return new handle.
  typedef InterfaceImpl<Strategy, unsigned int> Impl;
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl(type_, InstanceName(name), timeout) );
  return FactoryBuilder::FactoryPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace ManyToOne
} // namespace Filter
