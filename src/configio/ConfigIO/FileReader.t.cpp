/*
 * FileReader.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct FileReaderTestClass
{
};

typedef FileReaderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/FileReader");
} // unnamed namespace


namespace tut
{

// try opening some exising file, with read permission
template<>
template<>
void testObj::test<1>(void)
{
  const FileReader fr("testdata/file1.txt");
}

// open text file and check sizes
template<>
template<>
void testObj::test<2>(void)
{
  const FileReader fr("testdata/file1.txt");
  ensure_equals("invalid file size", fr.getSize(), 25u);
  ensure_equals("invalid string length of read file",
                strlen( fr.getString() ), 25u);
  ensure_equals("invalid stirng's content", string( fr.getString() ),
                "alice has a cat.\nor not?\n");
}

// open binary file and check size
template<>
template<>
void testObj::test<3>(void)
{
  const FileReader fr("testdata/file2.bin");
  ensure_equals("invalid file size", fr.getSize(), 28u);
}

// check if string and data pointers are equal
template<>
template<>
void testObj::test<4>(void)
{
  const FileReader  fr("testdata/file2.bin");
  const void       *data=fr.getData();
  const void       *str =fr.getString();
  ensure("pointers are not equal", data==str);
}

// check for exception when file cannot be read
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    FileReader fr("/etc/shadow");
    fail("opening file without permissions didn't caused an error");
  }
  catch(const System::Exception&)
  {
    // this is expected
  }
}

// check for exception when file does not exist
template<>
template<>
void testObj::test<6>(void)
{
  try
  {
    FileReader fr("/file/that/does/not/exist.txt");
    fail("opening non-exisitng file didn't caused an error");
  }
  catch(const System::Exception&)
  {
    // this is expected
  }
}

// check for exception when file does not exist, but can be created
template<>
template<>
void testObj::test<7>(void)
{
  try
  {
    const char *path="_file_that_does_not_exist_but_can_be_created.txt";
    unlink(path);           // just in case
    FileReader  fr(path);
    fail("opening non-exisitng file didn't caused an error");
  }
  catch(const System::Exception&)
  {
    // this is expected
  }
}

} // namespace tut
