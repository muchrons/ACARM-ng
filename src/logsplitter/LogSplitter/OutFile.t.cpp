/*
 * OutFile.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sstream>
#include <unistd.h>

#include "LogSplitter/OutFile.hpp"

using namespace std;
using namespace LogSplitter;

namespace
{
struct TestClass
{
  TestClass(void):
    tmpFile_("test_outfile.txt"),
    of_(tmpFile_, 0)
  {
  }

  ~TestClass(void)
  {
    unlink( tmpFile_.c_str() );
  }

  const string tmpFile_;
  OutFile      of_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("LogSplitter/OutFile");
} // unnamed namespace


namespace tut
{

// test appending something to file
template<>
template<>
void testObj::test<1>(void)
{
  of_<<"hello world";
  of_.flush();
}

// test appending new line
template<>
template<>
void testObj::test<2>(void)
{
  of_<<std::endl;
  of_.flush();
}

// test opening file that does not exist
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    OutFile of("/i/do/not/exist.tmp", 42);
    fail("c-tor didn't throw on unknown file");
  }
  catch(const runtime_error &)
  {
    // this is expected
  }
}

} // namespace tut
