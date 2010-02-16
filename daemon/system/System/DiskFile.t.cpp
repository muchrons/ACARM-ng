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

namespace System
{

struct DiskFileTestData
{
  DiskFileTestData(void):
    _dfW( std::string("/dev/null"),    O_WRONLY ),
    _dfR( std::string("/dev/urandom"), O_RDONLY ),
    _size( sizeof(_buf) ),
    _testFileName("THISisVeryLongfileNAMEthatshould_NOT_EVER_EXIST.tmp")
  {
    tut::ensure( testFileNameIsOk() );
  }

  // returns true, if test file name is free
  bool testFileNameIsOk(void) const
  {
    struct stat s;
    return stat( _testFileName.c_str(), &s )==-1 && errno==ENOENT;
  }

  DiskFile  _dfW;
  DiskFile  _dfR;
  char      _buf[1024];
  const int _size;
  const std::string _testFileName;
};

} // namespace System


namespace tut
{
typedef System::DiskFileTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/DiskFile");
}

using namespace System;

namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( _dfR.get()>=0 && _dfW.get()>=0 );

  ensure( read ( _dfR.get(), _buf, _size)==_size );   // read is ok.
  ensure( write( _dfW.get(), _buf, _size)==_size );   // write is ok.
}


// throw-on-error test
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    DiskFile( std::string("/somenonexisting/never/ever.file"), O_RDONLY );
    fail("DiskFile() didn't throw on unexisting file");
  }
  catch(const Exception&)
  {
    // should throw here
  }
}

// test unlinking
template<>
template<>
void testObj::test<3>(void)
{
  DiskFile  df( _testFileName );        // make some test file
  const int desc=df.get();              // get descriptor
  ensure( testFileNameIsOk()==false );  // now it should exist
  df.unlink();                          // remove file
  ensure( testFileNameIsOk()==true  );  // now it should NOT exist
  df.unlink();                          // miltiple calling should not fail
  ensure( desc==df.get() );             // file descriptor still active?
}

} // namespace tut

