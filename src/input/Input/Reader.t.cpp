/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Input/Reader.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestAlert: public Persistency::Alert
{
  TestAlert(void):
    Alert("name",
          AnalyzerPtr( new Analyzer) )
  {
  }
};

struct TestReader: public Reader
{
  typedef Reader::DataPtr DataPtr;

  explicit TestReader(bool &i):
    i_(i)
  {
  }

  virtual ~TestReader(void)
  {
    i_=true;
  }

  virtual DataPtr read(unsigned int)
  {
    return DataPtr(new TestAlert);
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

} // namespace tut
