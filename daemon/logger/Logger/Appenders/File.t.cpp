/*
 * File.t.cpp
 *
 */
#include <tut.h>
#include <cstdlib>

#include "Logger/Appenders/File.hpp"
#include "Logger/Appenders/FileUnlinker.t.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct FileTestClass
{
  FileTestClass():
    path_("mytest.log"),
    file_(path_),
    unlinker_(path_)
  {
  }

  const string       path_;
  File               file_;
  const FileUnlinker unlinker_;
};

typedef FileTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/File");
} // unnamed namespace


namespace tut
{

// test getting name in static way
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name returned",
                File::getThisTypeName(), string("File") );
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
  const string cmd="chmod 400 " + path_;
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

} // namespace tut
