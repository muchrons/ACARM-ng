/*
 * BackendProxy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Filter/BackendProxy.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Filter;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
  TestClass(void):
    conn_( BackendFactory::create("stubs", BackendFactory::Options() ) ),
    bp_( new BackendProxy(conn_, "sometest") )
  {
    assert( bp_.get()!=NULL );
    assert( conn_.get()!=NULL );
  }

  ConnectionPtrNN                 conn_;
  boost::scoped_ptr<BackendProxy> bp_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/BackendProxy");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

} // namespace tut
