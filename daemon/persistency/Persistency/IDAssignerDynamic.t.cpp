/*
 * IDAssignerDynamic.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Commons/Convert.hpp"
#include "Persistency/IDAssignerDynamic.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    t_( tioc_.createNewTransaction("test_assigner") ),
    dc_(t_, "next free GraphNode's ID")
  {
  }

  TestIOConnection       tioc_;
  IO::Transaction        t_;
  IODynamicConfigCounter dc_;
  IDAssignerDynamic      ad_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IDAssignerDynamic");
} // unnamed namespace


namespace tut
{

// test getting when no value is set value
template<>
template<>
void testObj::test<1>(void)
{
  ensure("initial value is NOT null", dc_.read(dc_.keyValue_).get()==NULL );
  ensure_equals("invalid value returned", ad_.assign(dc_).get(), 0u);
}

// test reading multiple times
template<>
template<>
void testObj::test<2>(void)
{
  ensure("initial value is NOT null", dc_.read(dc_.keyValue_).get()==NULL );
  ensure_equals("invalid value 1 returned", ad_.assign(dc_).get(), 0u);
  ensure_equals("invalid value 2 returned", ad_.assign(dc_).get(), 1u);
}

} // namespace tut
