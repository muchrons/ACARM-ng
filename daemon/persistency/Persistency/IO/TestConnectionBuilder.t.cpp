/*
 * TestConnectionBuilder.t.cpp
 *
 */
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/IO/TestConnectionBuilder.t.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace std;

namespace Persistency
{
namespace IO
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<BackendFactory, TestConnectionBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Persistency_IO_TestConnectionBuilder)


TestConnectionBuilder::TestConnectionBuilder(void):
  name_("persistencytest"),
  log_("persistency.io.persistencytest")
{
}

TestConnectionBuilder::FactoryPtr TestConnectionBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::IO::PersistencyTest");
  assert(g_rh.isRegistered() && "oops - registration failed");

  return TestConnectionBuilder::FactoryPtr(new TestIOConnection);
}

const TestConnectionBuilder::FactoryTypeName &TestConnectionBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace IO
} // namespace Persistency
