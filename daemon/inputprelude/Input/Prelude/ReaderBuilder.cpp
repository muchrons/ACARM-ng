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
  name_("prelude"),
  log_("input.prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options &opt) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(g_rh.isRegistered() && "oops - registration failed");

  // TODO: implement this

  const ConfigIO::InputConfig ic(name_, opt);
  for (Options::const_iterator o=opt.begin(); o!=opt.end(); ++o)
    {
      std::cerr << o->first << ' ' << o->second << std::endl;
    }
  // TODO: i'd suggest removign 'prelude' surfix - parameters are
  //       per-input any way so no collision is possible.
  const std::string &pprofile=ic["profile"];
  const std::string &pconfig=ic["config"];

  return ReaderBuilder::FactoryPtr( new Reader(pprofile, pconfig) );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Prelude
} // namespace Input
