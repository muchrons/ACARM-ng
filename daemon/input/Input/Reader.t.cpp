/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <memory>

#include "Input/Reader.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestReader: public Reader
{
  typedef Reader::DataPtr DataPtr;

  explicit TestReader(bool &i):
    Reader("testreader"),
    i_(i)
  {
  }

  virtual ~TestReader(void)
  {
    i_=true;
  }

  virtual DataPtr read(unsigned int)
  {
    return TestHelpers::Persistency::makeNewAlert();
  }

  bool &i_;
};

struct ReaderTestClass
{
  ReaderTestClass(void):
    i_(false),
    r_( new TestReader(i_) )
  {
  }

  bool             i_;
  auto_ptr<Reader> r_;
};

typedef ReaderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Reader");
} // unnamed namespace


namespace tut
{

// test virtual d-tor
template<>
template<>
void testObj::test<1>(void)
{
  ensure("pre-condition failed", i_==false);
  r_.reset(NULL);
  ensure("d-tor is not virtual", i_==true );
}

// test virtual read()
template<>
template<>
void testObj::test<2>(void)
{
  Reader::DataPtr     tmp  =r_->read();
  Persistency::Alert *alert=tmp.get();
  ensure("NULL pointer returned", alert!=NULL);
}

// smoke test for presence of smart-pointer declaration
template<>
template<>
void testObj::test<3>(void)
{
  ReaderPtrNN ptr( new TestReader(i_) );
  ensure("NULL pointer", ptr.get()!=NULL );
}

namespace
{

struct TestReader: public Reader
{
  TestReader(void):
    Reader("INVALID-chars-42")
  {
  }

  virtual DataPtr read(unsigned int)
  {
    return TestHelpers::Persistency::makeNewAlert();
  }
};

} // unnamed namespace

// test if not-allowed chars for logger name are accepted as name
template<>
template<>
void testObj::test<4>(void)
{
  TestReader tr;    // should not throw
}

} // namespace tut
