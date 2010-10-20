/*
 * isElementSane.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>

#include "Commons/Filesystem/isElementSane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  template<typename TEx>
  void ensureThrow(const path &p) const
  {
    try
    {
      isElementSane(p);   // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/isElementSane");
} // unnamed namespace


namespace tut
{

// test if sane dir is marked as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("normal dir not sane", isElementSane("testdata") );
}

// test if check for non-exisitng directory fails
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow<ExceptionFilesystemIO>("some/non/existing/dir");
}

// test id link to dir is not sane
template<>
template<>
void testObj::test<3>(void)
{
  ensure("link dir marked sane", isElementSane("testdata/dirSymlink")==false );
}

// test if dongling symlink throws
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/donglingSymlink");
}

// cur-dir test
template<>
template<>
void testObj::test<5>(void)
{
  ensure("'.' not sane", isElementSane(".") );
}

// test file symlink
template<>
template<>
void testObj::test<6>(void)
{
  ensure("file symlink marked sane", isElementSane("testdata/fileSymlink")==false );
}

// test if char device is not sain
template<>
template<>
void testObj::test<7>(void)
{
  ensure("char device marked sane", isElementSane("/dev/zero")==false );
}

// test if block device is not sain
template<>
template<>
void testObj::test<8>(void)
{
  // list of devices to check, since not all may be available in given filesystem. list will
  // probably expand in the future...
  vector<string> blockDevs;
  blockDevs.push_back("/dev/sda");
  blockDevs.push_back("/dev/hda");

  bool checked=false;
  for(vector<string>::const_iterator it=blockDevs.begin(); it!=blockDevs.end(); ++it)
    if( exists(*it) )
    {
      checked=true;
      ensure("block device marked sane", isElementSane(*it)==false );
      break;
    }

  ensure("no block device found - check the list in test case", checked);
}

} // namespace tut
