/*
 * Backtrace.cpp
 */
#include <sstream>
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cassert>

#include "System/Backtrace.hpp"
#include "System/AutoCptr.hpp"

using namespace std;

namespace System
{

namespace
{
AutoCptr<void*> memAlloc(const size_t num)
{
  const size_t    size=num*sizeof(void*);
  AutoCptr<void*> ptr( static_cast<void**>( malloc(size) ) );
  if(ptr.get()==NULL)
    throw runtime_error("memAlloc(): unable to allocate memory with malloc()");
  return ptr;
} // memAlloc()
} // unnmaed namespace

Backtrace::Backtrace(void)
{
  int             count =-1;
  int             size  =256;
  AutoCptr<void*> buffer( memAlloc(size) );

  // enlarge buffer untill whole read fits
  while(true)
  {
    count=backtrace(buffer.get(), size);    // try to read into buffer
    assert(count>0);
    if(count<size)                          // reading succeeded?
      break;
    // otherwise enlarge buffer and try again
    size  *=4;
    AutoCptr<void*> tmp( memAlloc(size) );
    buffer=tmp;
  } // while(true)

  // translate addresses to sybol names
  assert(count>0);
  assert(size>count);
  char **names=NULL;
  names=backtrace_symbols(buffer.get(), count);
  if(names==NULL)
    throw runtime_error("Backtrace::Backtrace(): backtrace_symbols() "
                        "failed: " + string( strerror(errno) ) );
  AutoCptr<char*> namesHolder(names);

  // rewrite table to vector
  assert( bt_.size()==0 );
  bt_.reserve(size);
  for(int i=0; i<count; ++i)
    bt_.push_back( names[i] );
}

std::string Backtrace::toString(void) const
{
  // create single, big string with multiple lines, representing backtrace.
  stringstream ss;
  for(const_iterator it=begin(); it!=end(); ++it)
    ss<<*it<<endl;

  return ss.str();
}

} // namespace System
