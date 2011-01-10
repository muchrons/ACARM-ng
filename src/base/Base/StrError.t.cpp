/*
 * StrError.t.cpp
 *
 */
#include <tut.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Base/StrError.hpp"

using namespace std;
using namespace Base;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/StrError");
} // unnamed namespace


namespace tut
{

// test string on no error.
template<>
template<>
void testObj::test<1>(void)
{
  void *mem=malloc(42);
  free(mem);
  const StrError se;
  ensure("error string too short", se.get().length() > 6 );
}

// test in case of error
template<>
template<>
void testObj::test<2>(void)
{
  const int fd=open("/this/file/does/not/exist", 0644, O_RDONLY);
  if(fd!=-1)
  {
    close(fd);
    fail("opening non-exisitng file didn't failed");
  }

  const StrError se;
  ensure("error string too short", se.get().length() > 6 );
}

} // namespace tut
