/*
 * IDAssigner.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <cassert>

#include "Commons/Convert.hpp"
#include "Persistency/Facades/IDAssigner.hpp"
#include "Persistency/Facades/TestIOConnectionParamMap.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades;

namespace
{

struct TestClass
{
  TestClass(void):
    conn_(new TestIOConnectionParamMap),
    t_( conn_->createNewTransaction("test_assigner") )
  {
  }

  IO::ConnectionPtrNN         conn_;
  IO::Transaction             t_;
  Facades::detail::IDAssigner ida_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/IDAssigner");
} // unnamed namespace


namespace tut
{

// test getting some value from singleton (smoke-test)
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid value returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), 0u);
}

// test getting meta-alert's ID when no value is set value
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid value returned", ida_.assignMetaAlertID(conn_, t_).get(), 0u);
}

// test assigning meta-alerts' ID multiple times
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid value 1 returned", ida_.assignMetaAlertID(conn_, t_).get(), 0u);
  ensure_equals("invalid value 2 returned", ida_.assignMetaAlertID(conn_, t_).get(), 1u);
}

// test getting analyzer's id when no value is set value
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid value returned", ida_.assignAnalyzerID(conn_, t_).get(), 0u);
}

// test assigning analyzers' ID multiple times
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid value 1 returned", ida_.assignAnalyzerID(conn_, t_).get(), 0u);
  ensure_equals("invalid value 2 returned", ida_.assignAnalyzerID(conn_, t_).get(), 1u);
}

} // namespace tut
