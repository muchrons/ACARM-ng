/*
 * ReaderBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Logger/Logger.hpp"

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
// TODO: this should be template function in Commons library
// helper that registers ReaderBuilder
bool registerBuilder(void)
{
  const Logger::Node node("input.prelude");
  try
  {
    LOGMSG_INFO(node, "registering Input::Prelude::ReaderBuilder");
    ReadersFactory::FactoryBuilderBaseAutoPtr ptr(new ReaderBuilder);
    ReadersFactory::registerBuilder(ptr);
  }
  catch(const std::exception &ex)
  {
    stringstream ss;
    ss<<"exception cought while registering Input::Prelude::ReaderBuilder: "
      <<ex.what();
    LOGMSG_FATAL(node, ss.str().c_str() );
    return false;
  }
  catch(...)
  {
    LOGMSG_FATAL(node, "unknown exception while registering "
                       "Input::Prelude::ReaderBuilder");
    return false;
  }

  return true;
} // register builder

const bool registered=registerBuilder();

} // unnamed namespace


ReaderBuilder::ReaderBuilder(void):
  name_("prelude"),
  log_("input.prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Input::Reader");
  assert(registered && "oops - registration failed");

  // TODO: implement this

  return ReaderBuilder::FactoryPtr( new Reader() );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Prelude
} // namespace Input
