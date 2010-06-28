/*
 * TestIsValid.t.hpp
 *
 */
#ifndef INCLUDE_TESTISVALID_T_HPP_FILE
#define INCLUDE_TESTISVALID_T_HPP_FILE

#include <cstring>
#include <sys/utsname.h>
#include <cassert>

namespace
{
bool isValid(const char *name)
{
  assert(name!=NULL);
  // localhost?
  if( strstr(name, "localhost")!=0 )
    return true;

  // machine name?
  struct utsname tmp;
  const int      ret=uname(&tmp);
  assert(ret==0 && "uname() failed in tests");
  if(ret!=0)
    return false;
  return strstr(name, tmp.nodename)!=0;
} // isValid()
} // unnamed namesapce

#endif
