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

namespace
{
unsigned int getTimeout(const FilterConfig &fc)
{
  try
  {
    const int          timeoutInt=boost::lexical_cast<int>( fc["timeout"] );
    const unsigned int timeout   =boost::numeric_cast<unsigned int>(timeoutInt);
    return timeout;
  }
  catch(const std::exception &ex)
  {
    // rethrow as more reasonable exception time
    throw ExceptionInvalidParameter( SYSTEM_SAVE_LOCATION, "timeout", ex.what() );
  }
  assert(!"code never reaches here");
} // getTimeout()

double getSimilarity(const FilterConfig &fc)
{
  try
  {
    const double similarity=boost::lexical_cast<double>( fc["similarity"] );
    return similarity;
  }
  catch(const std::exception &ex)
  {
    // rethrow as more reasonable exception time
    throw ExceptionInvalidParameter( SYSTEM_SAVE_LOCATION, "similarity", ex.what() );
  }
  assert(!"code never reaches here");
} // getSimilarity()
} // unnamed namespace

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert( g_rh.isRegistered() && "oops - registration failed" );

  const FilterConfig fc(type_, options);
  // read timeout
  const unsigned int timeout   =getTimeout(fc);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";
  // read similarity threshold
  const double       similarity=getSimilarity(fc);
  LOGMSG_INFO_S(log_)<<"setting similarity threshold to "<<similarity*100<<"%";

  // create and return new handle.
  typedef InterfaceImpl<Strategy, Strategy::Params> Impl;
  const Strategy::Params params(timeout, similarity);
  return FactoryBuilder::FactoryPtr( new Impl(type_, type_, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace ManyToMany
} // namespace Filter
