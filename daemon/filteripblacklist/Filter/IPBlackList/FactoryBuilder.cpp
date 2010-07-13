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

namespace
{
unsigned int parseUnsignedInt(const string &str)
{
  const int          i =boost::lexical_cast<int>(str);
  const unsigned int ui=boost::numeric_cast<unsigned int>(i);
  return ui;
} // parseUnsignedInt()
} // unnamed namespace

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(name_, options);
  const unsigned int refresh =parseUnsignedInt( fc["refresh"] );
  const unsigned int limit   =parseUnsignedInt( fc["limit"] );
  const double       priDelta=boost::lexical_cast<double>( fc["priorityDelta"] );
  LOGMSG_INFO_S(log_)<<"setting refresh interval to "<<refresh<<"[s]";
  LOGMSG_INFO_S(log_)<<"setting entries limit to "<<limit;
  Strategy::Parameters params(refresh, limit, priDelta);

  // create and return new handle.
  typedef InterfaceImpl<Strategy, Strategy::Parameters> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(name_, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace IPBlackList
} // namespace Filter
