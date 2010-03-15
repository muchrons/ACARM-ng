/*
 * ExceptionInvalidFormat.hpp
 *
 */
#include <ctype.h>
#include <cstring>
#include <cassert>

#include "Filter/IPBlackList/ExceptionInvalidFormat.hpp"

using namespace std;

namespace Filter
{
namespace IPBlackList
{

ExceptionInvalidFormat::ExceptionInvalidFormat(const Location &where,
                                               const char     *around,
                                               const char     *comment):
  Filter::Exception( where,
                     "ipblacklist",
                     cc("problem while parsing input near '",
                        makeAround(around),
                        "': ",
                        comment).c_str() )
{
}

string ExceptionInvalidFormat::makeAround(const char *in) const
{
  if(in==NULL)
    return "<NULL>";

  const size_t size     =64+1;
  char         tmp[size]={0};
  assert(tmp[0]     ==0);
  assert(tmp[size-1]==0);

  assert(in!=NULL);
  size_t i;
  for(i=0; *in!=0 && i<size-1; ++i, ++in)
  {
    if( isalnum(*in) || isblank(*in) )
      tmp[i]=*in;
    else
      tmp[i]='.';
  }
  // check if we finished on time
  assert(tmp[size-1]==0);

  // if string was too long, mark in in output string
  if(*in!=0 && i==size-1)
  {
    assert(i==size-1);
    strcpy(tmp+i-5, "[...]");
  }
  // return computed value
  assert(tmp[size-1]==0);
  assert( strlen(tmp)<size );
  return tmp;
}

} // namespace IPBlackList
} // namespace Filter
