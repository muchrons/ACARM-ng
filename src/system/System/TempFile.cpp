/*
 * TempFile.cpp
 *
 */
#include <stdlib.h>
#include <unistd.h>

#include "System/TempFile.hpp"

using namespace std;
using namespace boost;
using namespace System;

namespace
{
inline pair<string, shared_ptr<AutoDescriptor> > makeTempFilePair(void)
{
  char                       buf[]="/tmp/.TempFile_XXXXXX";
  shared_ptr<AutoDescriptor> fd( new AutoDescriptor( mkstemp(buf) ) );
  return pair<string, shared_ptr<AutoDescriptor> >( string(buf), fd );
} // makeTempFilePair()
} // unnamed namespace


namespace System
{

TempFile::TempFile(void):
  DiskFile( makeTempFilePair() )
{
  // file will be removed, so that after file descriptor will
  // be released, it will phisicly disapear from disk.
  unlink();
}

} // namespace System

