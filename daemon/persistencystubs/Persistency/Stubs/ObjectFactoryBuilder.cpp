/*
 * ObjectFactoryBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/BackendFactory.hpp"
#include "Persistency/Stubs/ObjectFactory.hpp"
#include "Persistency/Stubs/ObjectFactoryBuilder.hpp"

using namespace std;


namespace Persistency
{
namespace Stubs
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<BackendFactory, ObjectFactoryBuilder> g_rh;
} // unnamed namespace


ObjectFactoryBuilder::ObjectFactoryBuilder(void):
  name_("stubs"),
  log_("persistency.stubs")
{
}

ObjectFactoryBuilder::FactoryPtr ObjectFactoryBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::Stubs");
  assert(g_rh.isRegistered() && "oops - registration failed");
  // options are not needed here
  return ObjectFactoryBuilder::FactoryPtr(new ObjectFactory);
}

const ObjectFactoryBuilder::FactoryTypeName &ObjectFactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Stubs
} // namespace Persistency
