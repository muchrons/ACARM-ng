/*
 * SafeValue.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <boost/thread/thread.hpp>
#include <cassert>
#include <unistd.h>

#include "Base/Threads/SafeValue.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace std;
using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/SeftValue");
} // unnamed namespace


namespace tut
{

// creating deault value, if possible
template<>
template<>
void testObj::test<1>(void)
{
  const SafeValue<string> tmp;
  ensure_equals("invalid value", tmp.get(), "");
}

// try setting some value
template<>
template<>
void testObj::test<2>(void)
{
  SafeValue<std::string> tmp("narf");
  ensure_equals("invalid value set", tmp.get(), "narf");
}

namespace
{

class TestWriter
{
public:
  explicit TestWriter(SafeValue<std::string> *data):
    data_(data)
  {
    assert(data_!=NULL);
  }
  void operator()(void)
  {
    const int    seed=rand();
    stringstream ss;
    ss<<"hello "<<seed;
    for(int i=0; i<200; ++i)
    {
      data_->set( ss.str() );
      boost::thread::yield();  // switch context
    }
  }
private:
  SafeValue<string> *data_;
}; // class TestWriter

} // unnamed namespace

// test if locking works (i.e. check for races)
template<>
template<>
void testObj::test<3>(void)
{
  SafeValue<string> tmp;
  TestWriter tl1(&tmp);
  TestWriter tl2(&tmp);
  TestWriter tl3(&tmp);

  // start two threads
  Base::Threads::ThreadJoiner th1(tl1);
  Base::Threads::ThreadJoiner th2(tl2);
  Base::Threads::ThreadJoiner th3(tl3);

  // and join them
  th1->join();
  th2->join();
  th3->join();
}

// test copy-ctor
template<>
template<>
void testObj::test<4>(void)
{
  const SafeValue<string> v1("kszy");
  const SafeValue<string> v2(v1);
  ensure_equals("invalid value copyied", v2.get(), "kszy");
  ensure_equals("values differ", v1.get(), v2.get() );
}

// test assignment operator
template<>
template<>
void testObj::test<5>(void)
{
  const SafeValue<string> v1("kszy");
  SafeValue<string>       v2("abc");
  v2=v1;
  ensure_equals("invalid value assigned", v2.get(), "kszy");
  ensure_equals("values differ", v1.get(), v2.get() );
}

} // namespace tut
