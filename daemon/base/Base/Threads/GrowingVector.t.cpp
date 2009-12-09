/*
 * GrowingVector.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <cassert>

#include "Base/Threads/GrowingVector.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
  typedef GrowingVector<std::string> GV;

  TestClass(void):
    cgv_(gv_)
  {
  }

  GV        gv_;
  const GV &cgv_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/GrowingVector");
} // unnamed namespace


namespace tut
{

// tetsing initialized collection
template<>
template<>
void testObj::test<1>(void)
{
  ensure("const-iterators are invalid", cgv_.begin()==cgv_.end() );
  ensure("iterators are invalid",       gv_.begin() ==gv_.end()  );
  ensure_equals("invalid size", cgv_.size(), 0);
}

// adding element at the end.
template<>
template<>
void testObj::test<2>(void)
{
  gv_.push("hello world");
  ensure_equals("invalid size after adding one element", cgv_.size(), 1);
  ensure("const-iterators are invalid", cgv_.begin()!=cgv_.end() );
  ensure("iterators are invalid",       gv_.begin() !=gv_.end()  );
}

} // namespace tut
