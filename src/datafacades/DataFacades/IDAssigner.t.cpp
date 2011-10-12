/*
 * IDAssigner.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <cassert>

#include "Commons/Convert.hpp"
#include "DataFacades/IDAssigner.hpp"
#include "DataFacades/TestIOConnectionParamMap.t.hpp"

using namespace Persistency;
using namespace Facades;

namespace
{

struct TestClass
{
  TestClass(void):
    conn_(new TestIOConnectionParamMap),
    t_( conn_->createNewTransaction("test_assigner") ),
    maStart_( IDAssigner::get()->assignMetaAlertID(conn_, t_).get() ),
    anlStart_( IDAssigner::get()->assignAnalyzerID(conn_, t_).get() )
  {
  }

  IO::ConnectionPtrNN    conn_;
  IO::Transaction        t_;
  MetaAlert::ID::Numeric maStart_;
  Analyzer::ID::Numeric  anlStart_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/IDAssigner");
} // unnamed namespace


namespace tut
{

// test getting some value from singleton (smoke-test)
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid value returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), maStart_+1u);
}

// test getting meta-alert's ID when no value is set value
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid value returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), maStart_+1u);
}

// test assigning meta-alerts' ID multiple times
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid value 1 returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), maStart_+1u);
  ensure_equals("invalid value 2 returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), maStart_+2u);
}

// test getting analyzer's id when no value is set value
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid value returned", IDAssigner::get()->assignAnalyzerID(conn_, t_).get(), anlStart_+1u);
}

// test assigning analyzers' ID multiple times
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid value 1 returned", IDAssigner::get()->assignAnalyzerID(conn_, t_).get(), anlStart_+1u);
  ensure_equals("invalid value 2 returned", IDAssigner::get()->assignAnalyzerID(conn_, t_).get(), anlStart_+2u);
}

} // namespace tut
