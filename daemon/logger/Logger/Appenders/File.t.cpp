/*
 * File.t.cpp
 *
 */
#include <tut.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Appenders/File.hpp"
#include "Logger/Appenders/FileUnlinker.t.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct FileTestClass: private TestHelpers::TestBase
{
  FileTestClass():
    path_("mytest.log"),
    file_(path_),
    unlinker_(path_)
  {
  }

  const boost::filesystem::path path_;
  File                          file_;
  const FileUnlinker            unlinker_;
};

typedef FileTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Appenders/File");
} // unnamed namespace


namespace tut
{

// test getting name in static way
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name returned",
                File::getThisTypeName(), string("file") );
}

// test getting name from dynamic interface
template<>
template<>
void testObj::test<2>(void)
{
  const Appenders::Base &b=file_;
  ensure_equals("strings are not identical",
                b.getTypeName(), string( File::getThisTypeName() ) );
}

// check if addresses are physicaly indentical
template<>
template<>
void testObj::test<3>(void)
{
  const Appenders::Base &b=file_;
  ensure("pointers are not identical",
         b.getTypeName()==File::getThisTypeName() );
}

// test c-tor on file without access to
template<>
template<>
void testObj::test<4>(void)
{
  const string cmd="chmod 400 " + path_.string();
  const int    ret=system( cmd.c_str() );
  ensure_equals("unable to change file permission", ret, 0);

  try
  {
    File file(path_);
    fail("File() didn't throw on openning file without write permission");
  }
  catch(const ExceptionFileAccessError &)
  {
    // this is expected
  }
}

// test c-tor on non-exisitng file that cannot be accessed
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    File file("/this/path/does/not/exist/and/so/do/file.txt");
    fail("File() didn't throw on non-existing file");
  }
  catch(const ExceptionFileAccessError &)
  {
    // this is expected
  }
}

namespace
{
struct DirOwner: private boost::noncopyable
{
  explicit DirOwner(const char *path):
    path_(path)
  {
  }
  ~DirOwner(void)
  {
    rmdir(path_);   // in case it's directory
    unlink(path_);  // in case it's file
  }

private:
  const char *path_;
}; // struct DirOwner
} // unnamed namespace

// test reinit() call
template<>
template<>
void testObj::test<6>(void)
{
  const char *name="output.txt";
  DirOwner    dir(name);    // ensure test will do proper cleanup
  File f(name);

  unlink(name);             // remove file
  mkdir(name, 0755);        // make directory to block file opening
  try
  {
    f.reinit();             // should throw
    fail("reinit() didn't throw when file cannot be accessed");
  }
  catch(const ExceptionFileAccessError &)
  {
    // this is exected
  }

  rmdir(name);              // ok - now free this name
  f.reinit();               // this call must pass
}

} // namespace tut
