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
#include "Filter/Similarity/FactoryBuilder.hpp"
#include "Filter/Similarity/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace Similarity
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_Similarity_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("similarity"),
  log_("filter.samename.factorybuilder")
{
}

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_, options);
  // filter samename name
  const std::string &name      =fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  const unsigned int timeout   =Commons::Convert::to<unsigned int>(fc["timeout"]);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";
  const double       similarity=Commons::Convert::to<double>(fc["similarity"]);
  LOGMSG_INFO_S(log_)<<"setting similarity to "<<similarity*100<<"%";

  // create and return new handle.
  const Strategy::Params params(timeout, similarity);
  typedef InterfaceImpl<Strategy, Strategy::Params> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(type_, name, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace Similarity
} // namespace Filter
