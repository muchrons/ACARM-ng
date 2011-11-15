/*
 * CmdLineArgs.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "System/ignore.hpp"
#include "Commons/CmdLineArgs.hpp"

using namespace std;
using namespace Commons;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/CmdLineArgs");
} // unnamed namespace


namespace tut
{

// test empty args
template<>
template<>
void testObj::test<1>(void)
{
  const int          argc  =1;
  const char * const argv[]={"./a.out"};
  const CmdLineArgs  cla(argc, argv);
  ensure_equals("invalid arguemnts", cla.argc(), argc);
  ensure("arguments array is NULL", cla.argv()!=NULL);
  // 0
  ensure("argument is NULL", cla.argv()[0]!=NULL);
  ensure_equals("invalid argument", string(cla.argv()[0]), argv[0]);
}

// test one argument
template<>
template<>
void testObj::test<2>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "666"};
  const CmdLineArgs  cla(argc, argv);
  ensure_equals("invalid arguemnts", cla.argc(), argc);
  ensure("arguments array is NULL", cla.argv()!=NULL);
  // 0
  ensure("argument 0 is NULL", cla.argv()[0]!=NULL);
  ensure_equals("invalid argument 0", string(cla.argv()[0]), argv[0]);
  // 1
  ensure("argument 1 is NULL", cla.argv()[1]!=NULL);
  ensure_equals("invalid argument 1", string(cla.argv()[1]), argv[1]);
}

// test two arguments
template<>
template<>
void testObj::test<3>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "666", "narf"};
  const CmdLineArgs  cla(argc, argv);
  ensure_equals("invalid arguemnts", cla.argc(), argc);
  ensure("arguments array is NULL", cla.argv()!=NULL);
  // 0
  ensure("argument 0 is NULL", cla.argv()[0]!=NULL);
  ensure_equals("invalid argument 0", string(cla.argv()[0]), argv[0]);
  // 1
  ensure("argument 1 is NULL", cla.argv()[1]!=NULL);
  ensure_equals("invalid argument 1", string(cla.argv()[1]), argv[1]);
  // 2
  ensure("argument 2 is NULL", cla.argv()[2]!=NULL);
  ensure_equals("invalid argument 2", string(cla.argv()[2]), argv[2]);
}

// test error when argc==0
template<>
template<>
void testObj::test<4>(void)
{
  const int          argc  =0;
  const char * const argv[]={"./a.out"};
  try
  {
    CmdLineArgs cla(argc, argv);
    fail("c-tor didn't failed when to little argc was given");
  }
  catch(const CmdLineArgs::ExceptionInvalidArgument&)
  {
    // this is expected
  }
}

// test negative argc
template<>
template<>
void testObj::test<5>(void)
{
  const int          argc  =-2;
  const char * const argv[]={"./a.out"};
  try
  {
    CmdLineArgs cla(argc, argv);
    fail("c-tor didn't failed when on negative argc");
  }
  catch(const CmdLineArgs::ExceptionInvalidArgument&)
  {
    // this is expected
  }
}

// test NULL argv
template<>
template<>
void testObj::test<6>(void)
{
  const int            argc=2;
  const char * const * argv=NULL;
  try
  {
    CmdLineArgs cla(argc, argv);
    fail("c-tor didn't failed when on NULL argv");
  }
  catch(const CmdLineArgs::ExceptionInvalidArgument&)
  {
    // this is expected
  }
}

// test NULL argument
template<>
template<>
void testObj::test<7>(void)
{
  const int          argc  =4;
  const char * const argv[]={"./a.out", "arg1", NULL, "arg2"};
  try
  {
    CmdLineArgs cla(argc, argv);
    fail("c-tor didn't failed when on NULL argument");
  }
  catch(const CmdLineArgs::ExceptionInvalidArgument&)
  {
    // this is expected
  }
}

// test if argument are NOT const
template<>
template<>
void testObj::test<8>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "narf"};
  const CmdLineArgs  cla(argc, argv);
  // smoke test for edditing of argc
  int c=cla.argc();
  System::ignore(c);
  // smoke test for edditing single elements as well as pointers
  char **a=cla.argv();
  System::ignore(a);
}

} // namespace tut
