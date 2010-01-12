/*
 * Process.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Process.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    md5Str_("01234567890123456789012345678901"),
    md5_( MD5Sum::createFromString(md5Str_) ),
    pid_(42),
    uid_(13),
    user_("john"),
    args_("-a -b -c"),
    url_( makeNewReferenceURL() )
  {
  }

  const char              *md5Str_;
  const MD5Sum             md5_;
  const pid_t              pid_;
  const int                uid_;
  const Process::Username  user_;
  const char              *args_;
  ReferenceURLPtr          url_;
};

typedef TestClass TestClass;
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
  const Process ti("/path/to/file",
                   "file",
                   &md5_,
                   &pid_,
                   &uid_,
                   user_,
                   args_,
                   url_);
  ensure_equals("invalid path", ti.getPath().get(), string("/path/to/file") );
  ensure_equals("invalid name", ti.getName().get(), string("file")          );
  ensure_equals("invalid md5",  ti.getMD5()->get(), string(md5Str_)         );
}

// test NULL path
template<>
template<>
void testObj::test<2>(void)
{
  const Process ti(NULL,
                   "file",
                   &md5_,
                   &pid_,
                   &uid_,
                   user_,
                   args_,
                   url_);
  ensure("invalid path", ti.getPath().get()==NULL );
}

// test NULL MD5
template<>
template<>
void testObj::test<3>(void)
{
  const Process ti("/path/to/file",
                   "file",
                   NULL,
                   &pid_,
                   &uid_,
                   user_,
                   args_,
                   url_);
  ensure("invalid md5", ti.getMD5()==NULL );
}

// check NULL process name
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    const Process ti("/path/to/file",
                     NULL,
                     &md5_,
                     &pid_,
                     &uid_,
                     user_,
                     args_,
                     url_);
    fail("NULL file name accepted");
  }
  catch(const ExceptionNULLParameter&)
  {
    // this is expected
  }
}

// check NULL PID
template<>
template<>
void testObj::test<5>(void)
{
  const Process ti("/path/to/file",
                   "file",
                   &md5_,
                   NULL,
                   &uid_,
                   user_,
                   args_,
                   url_);
  ensure("invalid PID", ti.getPID()==NULL );
}

// check NULL UID
template<>
template<>
void testObj::test<6>(void)
{
  const Process ti("/path/to/file",
                   "file",
                   &md5_,
                   &pid_,
                   NULL,
                   user_,
                   args_,
                   url_);
  ensure("invalid UID", ti.getUID()==NULL );
}

// check NULL args
template<>
template<>
void testObj::test<7>(void)
{
  const Process ti("/path/to/file",
                   "file",
                   &md5_,
                   &pid_,
                   &uid_,
                   user_,
                   NULL,
                   url_);
  ensure("invalid arguments/paramters", ti.getParameters()==NULL );
}

// check NULL URL
template<>
template<>
void testObj::test<8>(void)
{
  const Process ti("/path/to/file",
                   "file",
                   &md5_,
                   &pid_,
                   &uid_,
                   user_,
                   args_,
                   ReferenceURLPtr() );
  ensure("invalid ", ti.getReferenceURL()==NULL );
}

} // namespace tut
