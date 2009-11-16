/*
 * MD5Sum.cpp
 *
 */
#include <cstring>
#include <ctype.h>
#include <cassert>

#include "Persistency/MD5Sum.hpp"

namespace Persistency
{

MD5Sum::MD5Sum(const char *str)
{
  // sanity check
  if(str==NULL)
    throw ExceptionNULLParameter("MD5Sum::MD5Sum", "str");

  // string must be of a given length
  if( strlen(str)!=MD5_STR_LEN )
    throw ExceptionInvalidMD5String("MD5Sum::MD5Sum", str);

  // check all chars
  for(int i=0; i<MD5_STR_LEN; ++i)
    if( !isdigit(str[i]) && !islower(str[i]) )
      throw ExceptionInvalidMD5String("MD5Sum::MD5Sum", str);

  // save data
  memcpy(str_, str, MD5_STR_LEN+1);
  assert( strcmp(str_, str)==0 );
}

} // namespace Persistency
