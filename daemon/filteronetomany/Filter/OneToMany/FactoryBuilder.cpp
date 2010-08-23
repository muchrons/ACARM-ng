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
#include "Filter/OneToMany/FactoryBuilder.hpp"
#include "Filter/OneToMany/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace OneToMany
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_OneToMany_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("onetomany"),
  log_("filter.onetomany.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_, options);
  // filter onetomany name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  const unsigned int timeout=Commons::Convert::to<unsigned int>(fc["timeout"]);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";

  // create and return new handle.
  typedef InterfaceImpl<Strategy, unsigned int> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(type_, name, timeout) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace OneToMany
} // namespace Filter
