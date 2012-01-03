/*
 * ParserCallback.t.cpp
 *
 */
#include <tut.h>

#include "Input/File/ParserCallback.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Persistency;
using namespace Input::File;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    t_( conn_->createNewTransaction("test_parser_callback") )
  {
  }

  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/File/ParserCallback");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor sequence
template<>
template<>
void testObj::test<1>(void)
{
  ParserCallback pc("testdata/test_short_alert.xml");
}

// test throw on non-existing file
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    ParserCallback pc("/i/do/not/exist");
    fail("c-tor didn't throw on invalid paramter");
  }
  catch(const Commons::Filesystem::Exception &)
  {
    // this is expected
  }
}

// test parsing some file
template<>
template<>
void testObj::test<3>(void)
{
  ParserCallback pc("testdata/test_short_alert.xml");
  pc.customAction(conn_, t_);
  ensure_equals("invalid name", pc.getAlert()->getName().get(), std::string("some alert") );
}

// test getting node w-out calling action first
template<>
template<>
void testObj::test<4>(void)
{
  ParserCallback pc("testdata/test_short_alert.xml");
  try
  {
    pc.getAlert();
    fail("getting alert didn't throw on invalid paramter");
  }
  catch(const Commons::ExceptionUnexpectedNULL &)
  {
    // this is expected
  }
}

// test parsing invalid file
template<>
template<>
void testObj::test<5>(void)
{
  ParserCallback pc("testdata/test_config.xml");        // TODO: hardcoded value
  try
  {
    pc.customAction(conn_, t_);
    fail("parsing didn't throw on invalid paramter");
  }
  catch(const RFCIO::IDMEF::Exception &)
  {
    // this is expected
  }
}

// open file that is read-only
// note: this is test for a bug from previous versions that opened input files r/w
template<>
template<>
void testObj::test<6>(void)
{
  ParserCallback pc("/etc/passwd"); // must not throw
}

} // namespace tut
