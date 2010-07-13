/*
 * FactoryBuilder.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/FilterConfig.hpp"
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
  name_("manytoone"),
  log_("filter.manytoone.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(name_, options);
  const int          timeoutInt=boost::lexical_cast<int>( fc["timeout"] );
  const unsigned int timeout   =boost::numeric_cast<unsigned int>(timeoutInt);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";

  // create and return new handle.
  typedef InterfaceImpl<Strategy, unsigned int> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(name_, timeout) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace ManyToOne
} // namespace Filter
