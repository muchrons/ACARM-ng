/*
 * DiskFile.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string>

#include "System/DiskFile.hpp"

using namespace System;

namespace
{

struct TestClass
{
  TestClass(void):
    dfW_( std::string("/dev/null"),    O_WRONLY ),
    dfR_( std::string("/dev/urandom"), O_RDONLY ),
    size_( sizeof(buf_) ),
    testFileName_("THISisVeryLongfileNAMEthatshould_NOT_EVER_EXIST.tmp")
  {
    tut::ensure( testFileNameIsOk() );
  }

  // returns true, if test file name is free
  bool testFileNameIsOk(void) const
  {
    struct stat s;
    return stat( testFileName_.c_str(), &s )==-1 && errno==ENOENT;
  }

  DiskFile          dfW_;
  DiskFile          dfR_;
  char              buf_[1024];
  const int         size_;
  const std::string testFileName_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/DiskFile");
}


namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( dfR_.get()>=0 && dfW_.get()>=0 );

  ensure( read ( dfR_.get(), buf_, size_)==size_ );   // read is ok.
  ensure( write( dfW_.get(), buf_, size_)==size_ );   // write is ok.
}


// throw-on-error test
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    DiskFile("/somenonexisting/never/ever.file", O_RDONLY);
    fail("DiskFile() didn't throw on unexisting file");
  }
  catch(const DiskFile::ExceptionCannotOpenFile &)
  {
    // should throw here
  }
}

// test unlinking
template<>
template<>
void testObj::test<3>(void)
{
  DiskFile  df(testFileName_);          // make some test file
  const int desc=df.get();              // get descriptor
  ensure( testFileNameIsOk()==false );  // now it should exist
  df.unlink();                          // remove file
  ensure( testFileNameIsOk()==true  );  // now it should NOT exist
  df.unlink();                          // miltiple calling should not fail
  ensure( desc==df.get() );             // file descriptor still active?
}

} // namespace tut

