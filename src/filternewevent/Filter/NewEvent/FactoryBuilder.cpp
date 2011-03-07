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
#include "Filter/NewEvent/FactoryBuilder.hpp"
#include "Filter/NewEvent/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace NewEvent
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_NewEvent_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("newevent"),
  log_("filter.newevent.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_, options);
  // filter newevent name
  const std::string    &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  const unsigned int timeout=Commons::Convert::to<unsigned int>(fc["timeout"]);
  const double       priDelta=Commons::Convert::to<double>( fc["priorityDelta"] );
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";
  LOGMSG_INFO_S(log_)<<"setting priority delta to "<<priDelta;
  Strategy::Parameters params(timeout, priDelta);
  // create and return new handle.
  typedef InterfaceImpl<Strategy, Strategy::Parameters> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(type_, name, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace NewEvent
} // namespace Filter
