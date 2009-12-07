/*
 * ObjectFactoryBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/BackendFactory.hpp"
#include "Persistency/Postgres/ObjectFactory.hpp"
#include "Persistency/Postgres/ObjectFactoryBuilder.hpp"

using namespace std;


namespace Persistency
{
namespace Postgres
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<BackendFactory, ObjectFactoryBuilder> g_rh;
} // unnamed namespace


ObjectFactoryBuilder::ObjectFactoryBuilder(void):
  name_("postgres"),
  log_("persistency.postgres")
{
}

ObjectFactoryBuilder::FactoryPtr ObjectFactoryBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::Postgres");
  assert(g_rh.isRegistered() && "oops - registration failed");

  // TODO: implement this

  return ObjectFactoryBuilder::FactoryPtr(new ObjectFactory);
}

const ObjectFactoryBuilder::FactoryTypeName &ObjectFactoryBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Postgres
} // namespace Persistency
