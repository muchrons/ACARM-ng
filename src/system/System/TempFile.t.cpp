/*
 * TempFile.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "System/TempFile.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/TempFile");
}

using namespace System;

namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  TempFile();   // temp file should be created now
  // and removed here
}

// try readinkg and writing from temporary test file
template<>
template<>
void testObj::test<2>(void)
{
  TempFile   tmp;
  DiskFile  *df=&tmp;       // this should also work
  int        fd=df->get();  // take file descirptor
  char       buf[512];      // temporary buffor for r/w operations
  const char str[]="alice has a cat :-)\n"; // test-string

  // write data
  int len=strlen(str);
  ensure("write error", write(fd, str, len)==len );
  // zero output buffer, as needed
  memset(buf, 0, sizeof(buf) );
  // read data
  ensure("seek error", lseek(fd, 0, SEEK_SET)==0 ); // go to the begin of file
  ensure("read error", read(fd, buf, len)==len   ); // perform read operation
  // check result
  ensure("strings does not match", strcmp(buf, str)==0 );
}

// test for resource leaks; this test is partialy manula as well:
// if there will be many temporary files in /tmp/ dir, it
// means that class doesn't cleanup after itself!
template<>
template<>
void testObj::test<3>(void)
{
  for(int i=0; i<100; ++i)
    TempFile();
}

} // namespace tut

