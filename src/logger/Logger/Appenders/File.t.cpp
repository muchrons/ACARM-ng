/*
 * File.t.cpp
 *
 */
#include <tut.h>

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
  const Base &b=file_;
  ensure_equals("strings are not identical",
                b.getTypeName(), string( File::getThisTypeName() ) );
}

// check if addresses are physicaly indentical
template<>
template<>
void testObj::test<3>(void)
{
  const Base &b=file_;
  ensure("pointers are not identical",
         b.getTypeName()==File::getThisTypeName() );
}

// try openeing some test file
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

} // namespace tut
