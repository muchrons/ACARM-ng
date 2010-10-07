/*
 * NewFile.t.cpp
 *
 */
#include <tut.h>
#include <fstream>

#include "Commons/Filesystem/NewFile.hpp"

using namespace std;
using namespace Commons::Filesystem;

namespace
{

struct Unlinker
{
  explicit Unlinker(const Path &path):
    path_(path)
  {
  }
  ~Unlinker(void)
  {
    try
    {
      boost::filesystem::remove(path_);
    }
    catch(...)
    {
      // nothing can be done here
    }
  }

private:
  const Path path_;
}; // struct Unlinker

struct TestClass
{
  void testThrow(const char *path) const
  {
    try
    {
      NewFile enf(path);
      tut::fail("c-tor didn't throw when conditions should not allow file opening");
    }
    catch(const ExceptionFileIO &)
    {
      // this is expected...
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/NewFile");
} // unnamed namespace


namespace tut
{

// test creating new file
template<>
template<>
void testObj::test<1>(void)
{
  const char    *path("some_test_file_of_NewFile_class.txt");
  Unlinker       u(path);
  NewFile  enf(path);
  ifstream       is(path, ios::binary);
  ensure("file does not exist", is.is_open() );
}

// test if creating new file will fail when file exists
template<>
template<>
void testObj::test<2>(void)
{
  const char    *path("some_test_source_to_link_of_NewFile_class.txt");
  Unlinker       u(path);
  NewFile  enf(path); // first instance
  testThrow(path);
}

// test fail if file is a sym-link
template<>
template<>
void testObj::test<3>(void)
{
  const char    *path("some_test_source_to_link_of_NewFile_class.txt");
  const char    *link("some_test_sym-link_of_NewFile_class.txt");
  Unlinker       u1(path);
  Unlinker       u2(link);
  NewFile  enf(path);
  ensure("can't create sym-link", symlink(path, link)==0 );
  testThrow(link);
}

// test fail if file is a hard-link to another
template<>
template<>
void testObj::test<4>(void)
{
  const char    *path("some_test_source_to_link_of_NewFile_class.txt");
  const char    *linkPath("some_test_hard-link_of_NewFile_class.txt");
  Unlinker       u1(path);
  Unlinker       u2(linkPath);
  NewFile  enf(path);
  ensure("can't create hard-link", link(path, linkPath)==0 );
  testThrow(linkPath);
}

} // namespace tut
