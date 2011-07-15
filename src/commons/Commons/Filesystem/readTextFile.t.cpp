/*
 * readTextFile.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/readTextFile.hpp"

using namespace std;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/readTextFile");
} // unnamed namespace


namespace tut
{

// test reading file with absolute path
template<>
template<>
void testObj::test<1>(void)
{
  boost::shared_array<char> data=readTextFile("/etc/passwd");
  ensure("got NULL pointer", data.get()!=NULL);
  ensure("file is empty", data[0]!=0);
}

// test reading non-existing file
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    readTextFile("/i/do/not/exist");
    fail("call didn't throw on error");
  }
  catch(const ExceptionFilesystemIO &)
  {
    // this is expected
  }
}

// test opening some file with relative path
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid file's content", readTextFile("testdata/one_line.txt").get(), string("how good to be evil!\n") );
}

// try reading multi-line file
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid file's content", readTextFile("testdata/multi_line.txt").get(), string("line1\nsecond line\n") );
}

// try reading empty file
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid file's content", readTextFile("testdata/empty_file.txt").get(), string("") );
}

} // namespace tut
