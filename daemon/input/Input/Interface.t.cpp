/*
 * Interface.t.cpp
 *
 */
#include <tut.h>
#include <unistd.h>

#include "Input/Interface.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestReader: public Reader
{
  TestReader(void):
    Reader("testreader", "testreadername")
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    usleep(50*1000);   // limit output a little...
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }
}; // struct TestReader

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    tr_(new TestReader),
    r_(tr_),
    conn_( createUserStub() ),
    t_( conn_->createNewTransaction("test_interface") )
  {
  }

  TestReader                       *tr_;
  ReaderPtrNN                       r_;
  Core::Types::AlertsFifo           output_;
  Persistency::IO::ConnectionPtrNN  conn_;
  Persistency::IO::Transaction      t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Interface");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor - d-tor should join thread
template<>
template<>
void testObj::test<1>(void)
{
  {
    Interface iface(r_, conn_, output_);
    while( output_.size()<2 )   // wait for few elements on output
      usleep(1*1000);
  }
  const size_t size=output_.size();
  usleep(100*1000);
  ensure_equals("thread didn't stop", output_.size(), size);
}

// test manual stopping thread
template<>
template<>
void testObj::test<2>(void)
{
  Interface iface(r_, conn_, output_);
  while( output_.size()<2 )   // wait for few elements on output
    usleep(1*1000);
  iface.stop();
  usleep(100*1000);
  const size_t size=output_.size();
  usleep(100*1000);
  ensure_equals("thread didn't stop", output_.size(), size);
}

// test pointer declaration's presence
template<>
template<>
void testObj::test<3>(void)
{
  InterfacePtrNN iface( new Interface(r_, conn_, output_) );
}


namespace
{
struct TestReaderName: public Reader
{
  TestReaderName(void):
    Reader("INVALID-42-chars_NARF", "somename")
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    usleep(50*1000);   // limit output a little...
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }
}; // struct TestReaderName
} // unnamed namespace

// test for old bug with name that is not valid logger name
template<>
template<>
void testObj::test<4>(void)
{
  ReaderPtrNN trn(new TestReaderName);
  Interface   iface(trn, conn_, output_);  // should not throw
}


namespace
{
struct TestReaderThrower: public Reader
{
  TestReaderThrower(void):
    Reader("reader.thrower", "throwerinput")
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    throw std::runtime_error("test exception: see what happens...");
  }
}; // struct TestReaderThrower
} // unnamed namespace

// test for old bug with name that is not valid logger name
template<>
template<>
void testObj::test<5>(void)
{
  ReaderPtrNN trn(new TestReaderThrower);
  Interface   iface(trn, conn_, output_);   // should not throw
  usleep(250*1000);                         // give thread some time to work
}

} // namespace tut
