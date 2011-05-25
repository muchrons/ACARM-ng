/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <memory>

#include "Input/Reader.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;
using namespace Core::Types::Proc;

namespace
{

struct TestReader: public Reader
{
  typedef Reader::DataPtr DataPtr;

  explicit TestReader(bool &i):
    Reader( TypeName("testreader"), InstanceName("testreadername") ),
    i_(i)
  {
  }

  virtual ~TestReader(void)
  {
    i_=true;
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }

  bool &i_;
};

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    i_(false),
    r_( new TestReader(i_) )
  {
  }

  bool             i_;
  auto_ptr<Reader> r_;
};

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
  Persistency::IO::ConnectionPtrNN        conn( createUserStub() );
  Persistency::IO::Transaction            t( conn->createNewTransaction("test_reader") );
  Persistency::Facades::AnalyzersCreator  ac;
  BackendFacade                           bf(conn, TypeName("testemall"), InstanceName("myname"), ac, "El'Ownerro");
  Reader::DataPtr                         tmp  =r_->read(bf);
  Persistency::Alert                     *alert=tmp.get();
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

struct TestReaderName: public Reader
{
  TestReaderName(void):
    Reader( TypeName("INVALID-chars-42"), InstanceName("readername") )
  {
  }
  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }
};

} // unnamed namespace

// test if not-allowed chars for logger name are accepted as name
template<>
template<>
void testObj::test<4>(void)
{
  TestReaderName tr;    // should not throw
}

} // namespace tut
