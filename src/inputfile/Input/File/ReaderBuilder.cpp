/*
 * ReaderBuilder.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"

#include "ConfigIO/InputConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/File/ReaderBuilder.hpp"
#include "Input/File/Reader.hpp"
#include "Input/Factory.hpp"

using namespace std;
using namespace Core::Types::Proc;

namespace Input
{
namespace File
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<Factory, ReaderBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Input_File_ReaderBuilder)


ReaderBuilder::ReaderBuilder(void):
  type_("file"),
  log_("input.file")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options &opt) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(g_rh.isRegistered() && "oops - registration failed");

  const ConfigIO::InputConfig   ic(type_.str(), opt);
  const string                  name    =ic["name"];
  const boost::filesystem::path fifoPath=ic["fifo"];

  return ReaderBuilder::FactoryPtr( new Reader(InstanceName(name), fifoPath) );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return type_.str();
}

} // namespace File
} // namespace Input
