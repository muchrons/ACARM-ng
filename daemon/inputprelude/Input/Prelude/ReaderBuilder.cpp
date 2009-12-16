/*
 * ReaderBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>
#include <string>

#include "Logger/Logger.hpp"

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/Prelude/ReaderBuilder.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/ReadersFactory.hpp"

using namespace std;


namespace Input
{
namespace Prelude
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<ReadersFactory, ReaderBuilder> g_rh;
} // unnamed namespace


ReaderBuilder::ReaderBuilder(void):
  name_("prelude"),
  log_("input.prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options & opt) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(g_rh.isRegistered() && "oops - registration failed");

  // TODO: implement this

  std::string profile=opt.find("prelude profile")->second;

  return ReaderBuilder::FactoryPtr( new Reader(profile) );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Prelude
} // namespace Input
