/*
 * DiskFile.mt.cpp
 *
 * tryes to write 2GB of data to disk.
 *
 */
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "System/DiskFile.hpp"

using namespace std;
using namespace System;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <file_to_store_over_2GB_of_trash>"<<endl;
    return 1;
  }

  try
  {
    char   onemb[1024*1024];            // 1MB of random data
    int    size=sizeof(onemb);
    size_t total=2*1024+10;             // 2GB + 10MB

    DiskFile  df( (string(argv[1])) );  // open file
    const int fd=df.get();              // get descriptor
    lseek64(fd, 0, SEEK_END);           // go to the end of file
    for(size_t i=0; i<total; ++i)       // try to write all
    {
      if(i%10==0)
        cerr<<"\r"<<i<<"MB ("<< (100*i)/total <<"%)";
      if( write(fd, onemb, size)!=size )// write do disk
        throw Exception( SYSTEM_SAVE_LOCATION,
                         string("write() error: ") + strerror(errno) );
    } // for(write>2GB)
    cerr<<endl;
  }
  catch(const Exception &ex)
  {
    cerr<<argv[0]<<": got System::Exception: "<<ex.what()<<endl;
    return 2;
  }
  catch(...)
  {
    cerr<<argv[0]<<": got unknown exception"<<endl;
    return 3;
  }

  return 0;
}

