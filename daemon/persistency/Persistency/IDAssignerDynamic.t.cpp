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


struct TestDynamicConfig: public Persistency::IO::DynamicConfig
{
public:
  explicit TestDynamicConfig(Persistency::IO::Transaction &t):
    Persistency::IO::DynamicConfig("some owner", t),
    keyValue_("next free GraphNode's ID"),
    isNull_(true),
    id_(1234567890)
  {
  }

  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    tut::ensure_equals("invalid key", key.get(), keyValue_);
    id_    =Commons::Convert::to<GraphNode::ID::Numeric>( value.get() );
    isNull_=false;
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    tut::ensure_equals("invalid key", key.get(), keyValue_);
    if(isNull_)
      return ValueNULL();
    return ValueNULL( Commons::Convert::to<std::string>(id_) );
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    tut::fail("this call should NOT be used at all in this context");
    return Value("???");
  }

  const std::string      keyValue_;
  bool                   isNull_;
  GraphNode::ID::Numeric id_;
}; // class TestDynamicConfig


struct TestClass
{
  TestClass(void):
    t_( tioc_.createNewTransaction("test_assigner") ),
    dc_(t_)
  {
  }

  TestIOConnection  tioc_;
  IO::Transaction   t_;
  TestDynamicConfig dc_;
  IDAssignerDynamic ad_;
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
