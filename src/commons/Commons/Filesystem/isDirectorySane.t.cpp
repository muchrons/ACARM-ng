/*
 * isDirectorySane.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/isDirectorySane");
} // unnamed namespace


namespace tut
{

// test if sane dir is marked as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("normal dir not sane", isDirectorySane("testdata") );
}

// test if check for non-exisitng directory fails
template<>
template<>
void testObj::test<2>(void)
{
  ensure("non-existing dir marked sane", isDirectorySane("some/non/existing/dir")==false );
}

// test if link to dir is not sane
template<>
template<>
void testObj::test<3>(void)
{
  ensure("link dir marked sane", isDirectorySane("testdata/dirSymlink")==false );
}

// test if dangling symlink not not sane
template<>
template<>
void testObj::test<4>(void)
{
  ensure("dangling symlink marked sane", isDirectorySane("testdata/danglingSymlink")==false );
}

// test if throws on file
template<>
template<>
void testObj::test<5>(void)
{
  ensure("file marked sane", isDirectorySane("testdata/file")==false );
}

// test if links in path are mathed as well
template<>
template<>
void testObj::test<6>(void)
{
  ensure("link inside path marked sane", isDirectorySane("testdata/dirSymlink/other_dir")==false );
}

// test some global directory
template<>
template<>
void testObj::test<7>(void)
{
  ensure("global directory not marked sane", isDirectorySane("/usr/lib") );
}

// test some relative directory path
template<>
template<>
void testObj::test<8>(void)
{
  ensure("relative directory not marked sane", isDirectorySane("testdata/dir") );
}

} // namespace tut
