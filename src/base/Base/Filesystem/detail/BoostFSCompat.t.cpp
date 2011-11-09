/*
 * BoostFSCompat.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Base/Filesystem/detail/BoostFSCompat.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Base::Filesystem::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Filesystem/detail/BoostFSCompat");
} // unnamed namespace


namespace tut
{

// test if file is ok
template<>
template<>
void testObj::test<1>(void)
{
  ensure("normal file not marked regular", isRegularFile("base.t") );
}

// test getting parent of a given element
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid parent path", parentPath("/some/path/myFile"), "/some/path");
}

} // namespace tut
