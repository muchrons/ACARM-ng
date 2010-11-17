/*
 * Full.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Trigger/Compose/Full.hpp"
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

factory tf("Trigger/Compose/Full");
} // unnamed namespace


namespace tut
{

// test creating report
template<>
template<>
void testObj::test<1>(void)
{
  std::stringstream ss;
  Full::append( ss, makeNewNode() );
  ensure("invalid report /1", strstr( ss.str().c_str(), "reporting triggered for meta-alert \"")!=NULL );
  ensure("invalid report /2", strstr( ss.str().c_str(), "\nsome meta-alert\n  some alert\n  some alert\n" )!=NULL );
  ensure("invalid report /3", strstr( ss.str().c_str(), "details: http://test.url/index.php?page=MetaAlert&sys_id=42" )!=NULL );
}

} // namespace tut
