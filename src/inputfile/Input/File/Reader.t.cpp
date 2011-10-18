/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <fstream>

#include "Input/File/Reader.hpp"
#include "System/Timer.hpp"
#include "Commons/Filesystem/isFifoSane.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;
using namespace Core::Types::Proc;
using namespace Input::File;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    fifo_("some_test_fifo"),
    r_(InstanceName("mynameisifileinput"), fifo_),
    conn_( IO::create() ),
    bf_(conn_, TypeName("my_backend_facade"), InstanceName("myname"), ac_)
  {
  }

  ~TestClass(void)
  {
    remove(fifo_);
  }

  const path                    fifo_;
  Reader                        r_;
  IO::ConnectionPtrNN           conn_;
  DataFacades::AnalyzersCreator ac_;
  Input::BackendFacade          bf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/File/Reader");
} // unnamed namespace


namespace tut
{

// test if type is valid
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", r_.getType().str(), "file");
}

// test test timeout
template<>
template<>
void testObj::test<2>(void)
{
  System::Timer t;
  ensure("invalid read", r_.read(bf_, 1).get()==NULL );
  ensure("timeout too short", t.elapsed()>=1 ); // timeout is the minumum
  ensure("timeout too long", t.elapsed()<3 );   // timeout*3 is the maximum
}

// test reading some alert
template<>
template<>
void testObj::test<3>(void)
{
  // write file name to queue
  fstream               queue( fifo_.string().c_str(), fstream::in|fstream::out );
  ensure("queue not opened", queue.is_open() );
  queue<<"testdata/test_short_alert.xml"<<endl;
  queue.flush();
  // try reading it
  const Reader::DataPtr dp=r_.read(bf_, 1);
  ensure("nothing has been read", dp.get()!=NULL );
  ensure_equals("invalid name", dp->getName().get(), string("some alert") );
}

// test if fifo is created at all
template<>
template<>
void testObj::test<4>(void)
{
  ensure("fifo has not been created", Commons::Filesystem::isFifoSane(fifo_) );
}

// test reading multiple times
template<>
template<>
void testObj::test<5>(void)
{
  // open queue
  fstream queue( fifo_.string().c_str(), fstream::in|fstream::out );
  ensure("queue not opened", queue.is_open() );
  for(int i=0; i<3; ++i)
  {
    // write file name
    queue<<"testdata/test_short_alert.xml"<<endl;
    queue.flush();
    // try reading it
    const Reader::DataPtr dp=r_.read(bf_, 1);
    ensure("nothing has been read", dp.get()!=NULL );
  }
}

// test if queue is creted, whe does not exist
template<>
template<>
void testObj::test<6>(void)
{
  remove(fifo_);
  Reader tmp(InstanceName("somename"), fifo_);
  ensure("not created", isFifoSane(fifo_) );
}

// test if queue is opened when created
template<>
template<>
void testObj::test<7>(void)
{
  ensure("not created in the first place", isFifoSane(fifo_) );
  // open queue
  fstream queue( fifo_.string().c_str(), fstream::in|fstream::out );
  ensure("queue not opened", queue.is_open() );
  // create new reader - it should open old queue, not overwirte it
  Reader tmp(InstanceName("somename"), fifo_);
  // write open command
  queue<<"testdata/test_short_alert.xml"<<endl;
  // test if file has been parsed and alert has been created
  ensure("queue has been overwritten instaed of opened", tmp.read(bf_, 1).get()!=NULL );
}

// test if name is valid
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals("invalid name", r_.getName().str(), "mynameisifileinput");
}

} // namespace tut
