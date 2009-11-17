/*
 * Process.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Process.hpp"
#include "Persistency/Process.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct ProcessTestClass
{
  ProcessTestClass(void):
    md5Str_("01234567890123456789012345678901"),
    md5_( MD5Sum::createFromString(md5Str_) )
  {
  }

  const char   *md5Str_;
  const MD5Sum  md5_;
};

typedef ProcessTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Process");
} // unnamed namespace


namespace tut
{

// check 2-arg c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const ProcessTestImpl ti("/path/to/file", "file", &md5_);
  ensure_equals("invalid path", ti.getPath().get(), string("/path/to/file") );
  ensure_equals("invalid name", ti.getName().get(), string("file")          );
  ensure_equals("invalid md5",  ti.getMD5()->get(), string(md5Str_)         );
}

// test NULL path
template<>
template<>
void testObj::test<2>(void)
{
  const ProcessTestImpl ti(NULL, "file", &md5_);
  ensure(       "invalid path", ti.getPath().get()==NULL            );
  ensure_equals("invalid name", ti.getName().get(), string("file")  );
  ensure_equals("invalid md5",  ti.getMD5()->get(), string(md5Str_) );
}

// test NULL MD5
template<>
template<>
void testObj::test<3>(void)
{
  const ProcessTestImpl ti("/path/to/file", "file", NULL);
  ensure_equals("invalid path", ti.getPath().get(), string("/path/to/file") );
  ensure_equals("invalid name", ti.getName().get(), string("file")          );
  ensure(       "invalid md5",  ti.getMD5()==NULL                           );
}

} // namespace tut
