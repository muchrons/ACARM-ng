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
#include "Filter/ManyToMany/FactoryBuilder.hpp"
#include "Filter/ManyToMany/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace ManyToMany
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_ManyToMany_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("manytomany"),
  log_("filter.manytomany.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert( g_rh.isRegistered() && "oops - registration failed" );

  const FilterConfig fc(type_.str(), options);
  // filter manytomany name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  // read timeout
  const unsigned int timeout   =Commons::Convert::to<unsigned int>(fc["timeout"]);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";
  // read similarity threshold
  const double       similarity=Commons::Convert::to<double>(fc["similarity"]);
  LOGMSG_INFO_S(log_)<<"setting similarity threshold to "<<similarity*100<<"%";

  // create and return new handle.
  typedef InterfaceImpl<Strategy, Strategy::Params> Impl;
  const Strategy::Params params(timeout, similarity);
  InterfaceWrapper::InterfaceAutoPtr ptr( new Impl(type_, InstanceName(name), params) );
  return FactoryBuilder::FactoryPtr(new InterfaceWrapper(ptr));
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace ManyToMany
} // namespace Filter
