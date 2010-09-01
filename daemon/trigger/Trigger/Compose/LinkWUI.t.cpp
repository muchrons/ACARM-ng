/*
 * LinkWUI.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Compose/LinkWUI.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Compose;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Compose/LinkWUI");
} // unnamed namespace


namespace tut
{

// test creating report
template<>
template<>
void testObj::test<1>(void)
{
  std::stringstream ss;
  LinkWUI::append( ss, makeNewLeaf() );
  ensure_equals("invalid report", ss.str(), "http://test.url/index.php?page=MetaAlertReport&id=303");
}

} // namespace tut
