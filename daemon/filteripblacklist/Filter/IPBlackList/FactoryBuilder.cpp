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
  name_("ipblacklist"),
  log_("filter.ipblacklist.factorybuilder")
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

  // create and return new handler.
  typedef InterfaceImpl<Strategy, unsigned int> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(name_, timeout) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace IPBlackList
} // namespace Filter
