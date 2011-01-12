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

MD5Sum::MD5Sum(const char *str):
  str_(str)
{
  // if str==NULL str_(str) should throw
  assert(str!=NULL);
  // strings should be binary equal
  assert( strcmp(str_.get(), str)==0 );

  // check all chars
  for(int i=0; i<MD5_STR_LEN; ++i)
    if( !isdigit(str_[i]) && !islower(str_[i]) )
      throw ExceptionInvalidMD5String(SYSTEM_SAVE_LOCATION, str_.get() );
}

} // namespace Persistency
