/*
 * ReaderBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Logger/Logger.hpp"

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/Prelude/ReaderBuilder.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/ReadersFactory.hpp"

using namespace std;

// TODO: add automatic registration to the factory

namespace Input
{
namespace Prelude
{

namespace
{
const Commons::Factory::RegistratorHelper<ReadersFactory, ReaderBuilder> g_rh;
} // unnamed namespace


ReaderBuilder::ReaderBuilder(void):
  name_("prelude"),
  log_("input.prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(g_rh.isRegistered() && "oops - registration failed");

  // TODO: implement this

  return ReaderBuilder::FactoryPtr( new Reader() );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Prelude
} // namespace Input
