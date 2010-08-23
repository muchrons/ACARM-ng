/*
 * ReaderBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>
#include <string>

#include "Logger/Logger.hpp"

#include "ConfigIO/InputConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/Prelude/ReaderBuilder.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/Factory.hpp"

using namespace std;


namespace Input
{
namespace Prelude
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<Factory, ReaderBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Input_Prelude_ReaderBuilder)


ReaderBuilder::ReaderBuilder(void):
  type_("prelude"),
  log_("input.prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options &opt) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const ConfigIO::InputConfig ic(type_, opt);

  const std::string &pname=ic["name"];
  const std::string &pprofile=ic["profile"];
  const std::string &pconfig=ic["config"];

  return ReaderBuilder::FactoryPtr( new Reader(pprofile, pname, pconfig) );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return type_;
}

} // namespace Prelude
} // namespace Input
