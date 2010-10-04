/*
 * AnalyzersMap.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Input/detail/AnalyzersMap.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Input::detail;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  AnalyzersMap am_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/detail/AnalyzersMap");
} // unnamed namespace


namespace tut
{

// test getting non-existing entry
template<>
template<>
void testObj::test<1>(void)
{
  ensure("valid entry returnd instead of NULL", am_.get("i don't exist").get()==NULL );
}

// test adding some entry
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::AnalyzerPtrNN analyzer=TestHelpers::Persistency::makeNewAnalyzer();
  am_.add("evilisgood", analyzer);
  ensure("NULL entry read", am_.get("evilisgood").get()!=NULL );
  ensure("invalid analyzer (copy?) has been returned", am_.get("evilisgood").get()==analyzer.get() );
}

// test adding the same entry twice
template<>
template<>
void testObj::test<3>(void)
{
  am_.add("evilisgood", TestHelpers::Persistency::makeNewAnalyzer() );
  try
  {
    am_.add("evilisgood", TestHelpers::Persistency::makeNewAnalyzer() );
    fail("add() didn't throw for non-unique key");
  }
  catch(const AnalyzersMap::ExceptionAddingAlreadyExistingKey &)
  {
    // this is expected
  }
}

} // namespace tut
