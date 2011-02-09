/*
 * FactoryBuilder.cpp
 *
 */
#include <cassert>
#include <boost/tokenizer.hpp>

#include "BuildProcess/ForceLink.hpp"
#include "ConfigIO/FilterConfig.hpp"
#include "Commons/Convert.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Filter/UsersMonitor/FactoryBuilder.hpp"
#include "Filter/UsersMonitor/Strategy.hpp"

using namespace std;
using namespace ConfigIO;
using namespace Core::Types::Proc;


namespace Filter
{
namespace UsersMonitor
{

namespace
{
const Commons::Factory::RegistratorHelper<Factory, FactoryBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Filter_UsersMonitor_FactoryBuilder)


FactoryBuilder::FactoryBuilder(void):
  type_("usersmonitor"),
  log_("filter.usersmonitor.factorybuilder")
{
}

namespace
{
inline Data::Names parseSkipNames(const Logger::Node &log, const std::string &in)
{
  LOGMSG_DEBUG_S(log)<<"parsing users to be skipped list: '"<<in<<"'";
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator    sep(" ");
  const Tokenizer    tokens(in, sep);
  Data::Names        skip;
  // got through all tokens
  for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
  {
    skip.push_back(*it);
    LOGMSG_INFO_S(log)<<"user '"<<*it<<"' added to skip list";
  } // for(tokens)
  return skip;
} // parseSkipNames()
} // unnamed namespace

FactoryBuilder::FactoryPtr FactoryBuilder::buildImpl(const Options &options) const
{
  LOGMSG_INFO(log_, "building filter's instance");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const FilterConfig fc(type_, options);
  // filter onetoone name
  const std::string &name=fc["name"];
  LOGMSG_INFO_S(log_)<<"setting filter \""<<getTypeName()<<"\" name to \""<<name<<"\"";
  const unsigned int timeout=Commons::Convert::to<unsigned int>(fc["timeout"]);
  LOGMSG_INFO_S(log_)<<"setting timeout to "<<timeout<<"[s]";
  const Data::Names &skip=parseSkipNames(log_,fc["skip"]);
  LOGMSG_INFO_S(log_)<<"setting "<<skip.size()<<" user names to skip";

  // create and return new handle.
  const Strategy::Parameters params(timeout, skip);
  typedef InterfaceImpl<Strategy, Strategy::Parameters> Impl;
  return FactoryBuilder::FactoryPtr( new Impl(type_, name, params) );
}

const FactoryBuilder::FactoryTypeName &FactoryBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace UsersMonitor
} // namespace Filter
