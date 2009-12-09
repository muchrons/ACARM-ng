/*
 * GrowingVector.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

#include "Base/Threads/GrowingVector.hpp"

using namespace Base::Threads;

namespace
{
typedef GrowingVector<std::string> GV;
typedef boost::shared_ptr<GV>      GVPtr;

struct TestClass
{
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

// test iterating thought collection
template<>
template<>
void testObj::test<3>(void)
{
  gv_.push("hello world");
  ensure_equals("invalid size after adding one element", cgv_.size(), 1);
  GV::const_iterator it =cgv_.begin();
  GV::const_iterator end=cgv_.end();
  ensure("const-iterators are invalid", it!=end);
  ensure_equals("invalid element", *it, "hello world");
  ++it;
  ensure("const-iterators to end are invalid", it==end);
}

// test if iterators remain valid after reallocation of collection's data
template<>
template<>
void testObj::test<4>(void)
{
  gv_.push("hello world");
  GV::const_iterator it=gv_.begin();
  ensure_equals("invalid element", *it, "hello world");
  // add many element, to be sure that backedn has been relocated at least once
  for(int i=0; i<1024; ++i)
    gv_.push("some noise");

  // this is smoke test - if objects have been reallocated and this iterator
  // points to nowhere, it will probably crash.
  ensure_equals("invalid element after collection resize", *it, "hello world");
}

// iterators should remain valid after collection dallocation
template<>
template<>
void testObj::test<5>(void)
{
  GVPtr col(new GV);
  assert(col.get()!=NULL);
  col->push("hello");
  GV::const_iterator it =col->begin();
  GV::const_iterator end=col->end();
  ensure_equals("invalid element", *it, "hello");
  // remove collection itself
  col=GVPtr(new GV);

  // this is smoke test - if objects have been reallocated and this iterator
  // points to nowhere, it will probably crash.
  ensure_equals("invalid element after collection removal", *it, "hello");
  // check the end of collection
  ++it;
  ensure("now we should be at collection's end", it==end);
}


namespace
{
struct CollectionWriter
{
  explicit CollectionWriter(GVPtr data):
    data_(data)
  {
    assert(data_.get()!=NULL);
    data_->push("anything");
  }

  void operator()(void)
  {
    // do a series of iterations
    for(int i=0; i<300; ++i)
    {
      // iterate tgrough colleciton changing some elemtns
      for(GV::iterator it=data_->begin(), end=data_->end(); it!=end; ++it)
        if( ( reinterpret_cast<long>(it->c_str()) )&0x1000 )    // randomly change content
          *it=*it+"change ";
        else
          boost::thread::yield();

      if(i%101)
        data_->push("some new data");
    }
  }

  GVPtr data_;
}; // struct CollectionWriter
} // unnamed namespace

// run smoke test for reading/writing from multiple threads
template<>
template<>
void testObj::test<6>(void)
{
  GVPtr            data(new GV);
  CollectionWriter cw(data);
  // run writings to the same collection from multiple threads
  boost::thread th1(cw);
  boost::thread th2(cw);
  boost::thread th3(cw);
  boost::thread th4(cw);

  // wait for them
  th1.join();
  th2.join();
  th3.join();
  th4.join();
}

// test copy c-tor.
template<>
template<>
void testObj::test<7>(void)
{
  // TODO
}

} // namespace tut
