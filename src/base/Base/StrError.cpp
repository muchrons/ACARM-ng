/*
 * StrError.cpp
 *
 */
#include <cstring>
#include <errno.h>

#include "Base/StrError.hpp"

namespace Base
{

namespace
{
inline std::string toString(const int e)
{
  char        buf[256];
  const char *tmp=strerror_r(e, buf, sizeof(buf) );
  if(tmp==NULL)
    return "<NULL>";
  return tmp;
} // toString()
} // unnamed namespace

StrError::StrError(void):
  msg_( toString(errno) )
{
}

} // namespace Base
